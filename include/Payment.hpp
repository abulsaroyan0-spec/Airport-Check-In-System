#pragma once

#include "Booking.hpp"

#include <memory>
#include <string>

enum class PaymentMethod {
    Cash,
    Card,
    Online
};

enum class PaymentStatus {
    Pending,
    Paid,
    Failed,
    Refunded
};

class Payment {
private:
    std::string transactionId;

    std::shared_ptr<Booking> booking;

    double amount;
    std::string currency;

    PaymentMethod method;
    PaymentStatus status;

public:
    Payment(
        const std::string& transactionId,
        std::shared_ptr<Booking> booking,
        double amount,
        const std::string& currency,
        PaymentMethod method
    );

    const std::string& getTransactionId() const;

    const std::shared_ptr<Booking>&
    getBooking() const;

    double getAmount() const;

    const std::string& getCurrency() const;

    PaymentMethod getMethod() const;

    PaymentStatus getStatus() const;

    bool isPaid() const;

    void process();

    void fail();

    void refund();

    std::string getMethodString() const;

    std::string getStatusString() const;

    void print() const;
};