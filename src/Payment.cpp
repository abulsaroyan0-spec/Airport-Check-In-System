#include "Payment.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

Payment::Payment(
    const std::string& transactionId,
    std::shared_ptr<Booking> booking,
    double amount,
    const std::string& currency,
    PaymentMethod method
)
    : transactionId(transactionId),
      booking(std::move(booking)),
      amount(amount),
      currency(currency),
      method(method),
      status(PaymentStatus::Pending)
{
    if (transactionId.empty()) {
        throw std::invalid_argument(
            "Transaction ID cannot be empty"
        );
    }

    if (!this->booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    if (amount < 0.0) {
        throw std::invalid_argument(
            "Payment amount cannot be negative"
        );
    }

    if (currency.empty()) {
        throw std::invalid_argument(
            "Currency cannot be empty"
        );
    }
}

const std::string&
Payment::getTransactionId() const
{
    return transactionId;
}

const std::shared_ptr<Booking>&
Payment::getBooking() const
{
    return booking;
}

double Payment::getAmount() const
{
    return amount;
}

const std::string&
Payment::getCurrency() const
{
    return currency;
}

PaymentMethod Payment::getMethod() const
{
    return method;
}

PaymentStatus Payment::getStatus() const
{
    return status;
}

bool Payment::isPaid() const
{
    return status == PaymentStatus::Paid;
}

void Payment::process()
{
    if (status == PaymentStatus::Paid) {
        throw std::runtime_error(
            "Payment is already completed"
        );
    }

    if (status == PaymentStatus::Refunded) {
        throw std::runtime_error(
            "Refunded payment cannot be processed again"
        );
    }

    if (booking->getStatus() ==
        BookingStatus::Cancelled) {

        throw std::runtime_error(
            "Cannot process payment for cancelled booking"
        );
    }

    status = PaymentStatus::Paid;
}

void Payment::fail()
{
    if (status == PaymentStatus::Paid) {
        throw std::runtime_error(
            "Paid transaction cannot be marked as failed"
        );
    }

    if (status == PaymentStatus::Refunded) {
        throw std::runtime_error(
            "Refunded transaction cannot be marked as failed"
        );
    }

    status = PaymentStatus::Failed;
}

void Payment::refund()
{
    if (status != PaymentStatus::Paid) {
        throw std::runtime_error(
            "Only paid transactions can be refunded"
        );
    }

    status = PaymentStatus::Refunded;
}

std::string Payment::getMethodString() const
{
    switch (method) {

        case PaymentMethod::Cash:
            return "Cash";

        case PaymentMethod::Card:
            return "Card";

        case PaymentMethod::Online:
            return "Online";
    }

    return "Unknown";
}

std::string Payment::getStatusString() const
{
    switch (status) {

        case PaymentStatus::Pending:
            return "Pending";

        case PaymentStatus::Paid:
            return "Paid";

        case PaymentStatus::Failed:
            return "Failed";

        case PaymentStatus::Refunded:
            return "Refunded";
    }

    return "Unknown";
}

void Payment::print() const
{
    std::cout
        << "\n========================================\n"
        << "          PAYMENT INFORMATION\n"
        << "========================================\n";

    std::cout
        << "Transaction ID: "
        << transactionId
        << '\n';

    std::cout
        << "Booking Code:   "
        << booking->getBookingCode()
        << '\n';

    std::cout
        << "Passenger:      "
        << booking->getPassenger()->getFullName()
        << '\n';

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Amount:         "
        << amount
        << ' '
        << currency
        << '\n';

    std::cout
        << "Method:         "
        << getMethodString()
        << '\n';

    std::cout
        << "Status:         "
        << getStatusString()
        << '\n';

    std::cout
        << "========================================\n";
}