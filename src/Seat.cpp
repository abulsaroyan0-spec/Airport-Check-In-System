#include "Seat.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>

namespace {

bool isValidSeatNumber(const std::string& seatNumber)
{
    if (seatNumber.size() < 2 || seatNumber.size() > 4) {
        return false;
    }

    const char seatLetter = seatNumber.back();

    if (!std::isalpha(
            static_cast<unsigned char>(seatLetter))) {
        return false;
    }

    for (std::size_t i = 0;
         i < seatNumber.size() - 1;
         ++i) {

        if (!std::isdigit(
                static_cast<unsigned char>(
                    seatNumber[i]))) {
            return false;
        }
    }

    return true;
}

}

Seat::Seat(
    const std::string& seatNumber,
    SeatClass seatClass
)
    : seatNumber(seatNumber),
      seatClass(seatClass),
      occupied(false)
{
    if (!isValidSeatNumber(seatNumber)) {
        throw std::invalid_argument(
            "Invalid seat number"
        );
    }
}

const std::string& Seat::getSeatNumber() const
{
    return seatNumber;
}

SeatClass Seat::getSeatClass() const
{
    return seatClass;
}

bool Seat::isOccupied() const
{
    return occupied;
}

bool Seat::isAvailable() const
{
    return !occupied;
}

void Seat::occupy()
{
    if (occupied) {
        throw std::runtime_error(
            "Seat is already occupied"
        );
    }

    occupied = true;
}

void Seat::release()
{
    if (!occupied) {
        throw std::runtime_error(
            "Seat is already available"
        );
    }

    occupied = false;
}

std::string Seat::getSeatClassString() const
{
    switch (seatClass) {

        case SeatClass::Economy:
            return "Economy";

        case SeatClass::PremiumEconomy:
            return "Premium Economy";

        case SeatClass::Business:
            return "Business";

        case SeatClass::FirstClass:
            return "First Class";
    }

    return "Unknown";
}

void Seat::print() const
{
    std::cout
        << "Seat Number: "
        << seatNumber
        << '\n'

        << "Class: "
        << getSeatClassString()
        << '\n'

        << "Status: "
        << (occupied
                ? "Occupied"
                : "Available")
        << '\n';
}