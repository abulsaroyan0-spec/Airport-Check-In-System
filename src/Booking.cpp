#include "Booking.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

Booking::Booking(
    const std::string& bookingCode,
    std::shared_ptr<Passenger> passenger,
    std::shared_ptr<Flight> flight
)
    : bookingCode(bookingCode),
      passenger(std::move(passenger)),
      flight(std::move(flight)),
      seat(nullptr),
      status(BookingStatus::Confirmed)
{
    if (bookingCode.empty()) {
        throw std::invalid_argument(
            "Booking code cannot be empty"
        );
    }

    if (!this->passenger) {
        throw std::invalid_argument(
            "Passenger cannot be null"
        );
    }

    if (!this->flight) {
        throw std::invalid_argument(
            "Flight cannot be null"
        );
    }
}

const std::string& Booking::getBookingCode() const
{
    return bookingCode;
}

const std::shared_ptr<Passenger>&
Booking::getPassenger() const
{
    return passenger;
}

const std::shared_ptr<Flight>&
Booking::getFlight() const
{
    return flight;
}

const std::shared_ptr<Seat>&
Booking::getSeat() const
{
    return seat;
}

const std::vector<std::shared_ptr<Luggage>>&
Booking::getLuggageList() const
{
    return luggageList;
}

BookingStatus Booking::getStatus() const
{
    return status;
}

bool Booking::hasSeat() const
{
    return seat != nullptr;
}

bool Booking::hasLuggage() const
{
    return !luggageList.empty();
}

std::size_t Booking::getLuggageCount() const
{
    return luggageList.size();
}

double Booking::getTotalLuggageWeight() const
{
    double totalWeight = 0.0;

    for (const auto& luggage : luggageList) {
        totalWeight += luggage->getWeight();
    }

    return totalWeight;
}

double Booking::getTotalExtraLuggageFee() const
{
    double totalFee = 0.0;

    for (const auto& luggage : luggageList) {
        totalFee += luggage->calculateExtraFee();
    }

    return totalFee;
}

void Booking::assignSeat(
    std::shared_ptr<Seat> seat
)
{
    if (!seat) {
        throw std::invalid_argument(
            "Seat cannot be null"
        );
    }

    if (status == BookingStatus::Cancelled) {
        throw std::runtime_error(
            "Cannot assign seat to cancelled booking"
        );
    }

    if (status == BookingStatus::Completed) {
        throw std::runtime_error(
            "Cannot assign seat to completed booking"
        );
    }

    if (this->seat) {
        throw std::runtime_error(
            "Booking already has a seat"
        );
    }

    if (!seat->isAvailable()) {
        throw std::runtime_error(
            "Selected seat is not available"
        );
    }

    seat->occupy();

    this->seat = std::move(seat);
}

void Booking::removeSeat()
{
    if (!seat) {
        throw std::runtime_error(
            "Booking does not have a seat"
        );
    }

    if (status == BookingStatus::CheckedIn) {
        throw std::runtime_error(
            "Cannot remove seat after check-in"
        );
    }

    if (status == BookingStatus::Completed) {
        throw std::runtime_error(
            "Cannot remove seat from completed booking"
        );
    }

    if (seat->isOccupied()) {
        seat->release();
    }

    seat.reset();
}

void Booking::addLuggage(
    std::shared_ptr<Luggage> luggage
)
{
    if (!luggage) {
        throw std::invalid_argument(
            "Luggage cannot be null"
        );
    }

    if (status == BookingStatus::Cancelled) {
        throw std::runtime_error(
            "Cannot add luggage to cancelled booking"
        );
    }

    if (status == BookingStatus::Completed) {
        throw std::runtime_error(
            "Cannot add luggage to completed booking"
        );
    }

    for (const auto& existingLuggage : luggageList) {

        if (existingLuggage->getTagNumber() ==
            luggage->getTagNumber()) {

            throw std::runtime_error(
                "Luggage with this tag number already exists"
            );
        }
    }

    luggageList.push_back(
        std::move(luggage)
    );
}

void Booking::removeLuggage(
    const std::string& tagNumber
)
{
    if (tagNumber.empty()) {
        throw std::invalid_argument(
            "Tag number cannot be empty"
        );
    }

    if (status == BookingStatus::CheckedIn) {
        throw std::runtime_error(
            "Cannot remove luggage after check-in"
        );
    }

    for (auto iterator = luggageList.begin();
         iterator != luggageList.end();
         ++iterator) {

        if ((*iterator)->getTagNumber() ==
            tagNumber) {

            luggageList.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Luggage not found: " + tagNumber
    );
}

std::shared_ptr<Luggage> Booking::findLuggage(
    const std::string& tagNumber
) const
{
    for (const auto& luggage : luggageList) {

        if (luggage->getTagNumber() ==
            tagNumber) {

            return luggage;
        }
    }

    throw std::runtime_error(
        "Luggage not found: " + tagNumber
    );
}

void Booking::checkIn()
{
    if (status == BookingStatus::Cancelled) {
        throw std::runtime_error(
            "Cannot check in a cancelled booking"
        );
    }

    if (status == BookingStatus::Completed) {
        throw std::runtime_error(
            "Booking is already completed"
        );
    }

    if (status == BookingStatus::CheckedIn) {
        throw std::runtime_error(
            "Passenger is already checked in"
        );
    }

    if (!seat) {
        throw std::runtime_error(
            "Seat must be selected before check-in"
        );
    }

    for (const auto& luggage : luggageList) {

        if (!luggage->isCheckedIn()) {
            luggage->checkIn();
        }
    }

    status = BookingStatus::CheckedIn;
}

void Booking::cancel()
{
    if (status == BookingStatus::Cancelled) {
        throw std::runtime_error(
            "Booking is already cancelled"
        );
    }

    if (status == BookingStatus::Completed) {
        throw std::runtime_error(
            "Completed booking cannot be cancelled"
        );
    }

    if (seat && seat->isOccupied()) {
        seat->release();
    }

    seat.reset();

    for (const auto& luggage : luggageList) {

        if (luggage->isCheckedIn()) {
            luggage->cancelCheckIn();
        }
    }

    status = BookingStatus::Cancelled;
}

void Booking::complete()
{
    if (status != BookingStatus::CheckedIn) {
        throw std::runtime_error(
            "Booking must be checked in before completion"
        );
    }

    status = BookingStatus::Completed;
}

std::string Booking::getStatusString() const
{
    switch (status) {

        case BookingStatus::Confirmed:
            return "Confirmed";

        case BookingStatus::CheckedIn:
            return "Checked In";

        case BookingStatus::Cancelled:
            return "Cancelled";

        case BookingStatus::Completed:
            return "Completed";
    }

    return "Unknown";
}

void Booking::print() const
{
    std::cout
        << "\n========================================\n"
        << "            BOOKING INFORMATION\n"
        << "========================================\n";

    std::cout
        << "Booking Code: "
        << bookingCode
        << '\n';

    std::cout
        << "Passenger: "
        << passenger->getFullName()
        << '\n';

    std::cout
        << "Passport: "
        << passenger->getPassport().getPassportNumber()
        << '\n';

    std::cout
        << "Flight: "
        << flight->getFlightNumber()
        << '\n';

    std::cout
        << "Route: "
        << flight->getRoute()
        << '\n';

    std::cout
        << "Departure: "
        << flight->getDepartureDate()
        << ' '
        << flight->getDepartureTime()
        << '\n';

    std::cout
        << "Status: "
        << getStatusString()
        << '\n';

    std::cout
        << "Seat: ";

    if (seat) {
        std::cout
            << seat->getSeatNumber()
            << " ("
            << seat->getSeatClassString()
            << ")\n";
    } else {
        std::cout
            << "Not selected\n";
    }

    std::cout
        << "----------------------------------------\n"
        << "Luggage Count: "
        << getLuggageCount()
        << '\n';

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Total Luggage Weight: "
        << getTotalLuggageWeight()
        << " kg\n";

    std::cout
        << "Extra Luggage Fee: $"
        << getTotalExtraLuggageFee()
        << '\n';

    std::cout
        << "========================================\n";
}