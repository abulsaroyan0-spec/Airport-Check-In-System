#include "BoardingPass.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

bool isValidTime(const std::string& time)
{
    if (time.size() != 5) {
        return false;
    }

    if (time[2] != ':') {
        return false;
    }

    if (!std::isdigit(
            static_cast<unsigned char>(time[0])) ||
        !std::isdigit(
            static_cast<unsigned char>(time[1])) ||
        !std::isdigit(
            static_cast<unsigned char>(time[3])) ||
        !std::isdigit(
            static_cast<unsigned char>(time[4]))) {

        return false;
    }

    const int hours =
        (time[0] - '0') * 10 +
        (time[1] - '0');

    const int minutes =
        (time[3] - '0') * 10 +
        (time[4] - '0');

    return hours >= 0 &&
           hours <= 23 &&
           minutes >= 0 &&
           minutes <= 59;
}

}

BoardingPass::BoardingPass(
    const std::string& boardingPassNumber,
    std::shared_ptr<Booking> booking,
    const std::string& boardingTime,
    int boardingGroup
)
    : boardingPassNumber(boardingPassNumber),
      booking(std::move(booking)),
      boardingTime(boardingTime),
      boardingGroup(boardingGroup),
      issued(false)
{
    if (boardingPassNumber.empty()) {
        throw std::invalid_argument(
            "Boarding pass number cannot be empty"
        );
    }

    if (!this->booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    if (!isValidTime(boardingTime)) {
        throw std::invalid_argument(
            "Invalid boarding time"
        );
    }

    if (boardingGroup <= 0) {
        throw std::invalid_argument(
            "Boarding group must be greater than zero"
        );
    }
}

const std::string&
BoardingPass::getBoardingPassNumber() const
{
    return boardingPassNumber;
}

const std::shared_ptr<Booking>&
BoardingPass::getBooking() const
{
    return booking;
}

const std::string&
BoardingPass::getBoardingTime() const
{
    return boardingTime;
}

int BoardingPass::getBoardingGroup() const
{
    return boardingGroup;
}

bool BoardingPass::isIssued() const
{
    return issued;
}

void BoardingPass::setBoardingTime(
    const std::string& boardingTime
)
{
    if (!isValidTime(boardingTime)) {
        throw std::invalid_argument(
            "Invalid boarding time"
        );
    }

    if (issued) {
        throw std::runtime_error(
            "Cannot change boarding time after boarding pass is issued"
        );
    }

    this->boardingTime = boardingTime;
}

void BoardingPass::setBoardingGroup(
    int boardingGroup
)
{
    if (boardingGroup <= 0) {
        throw std::invalid_argument(
            "Boarding group must be greater than zero"
        );
    }

    if (issued) {
        throw std::runtime_error(
            "Cannot change boarding group after boarding pass is issued"
        );
    }

    this->boardingGroup = boardingGroup;
}

void BoardingPass::issue()
{
    if (issued) {
        throw std::runtime_error(
            "Boarding pass is already issued"
        );
    }

    if (booking->getStatus() !=
        BookingStatus::CheckedIn) {

        throw std::runtime_error(
            "Passenger must be checked in before issuing boarding pass"
        );
    }

    if (!booking->hasSeat()) {
        throw std::runtime_error(
            "Passenger must have a seat before issuing boarding pass"
        );
    }

    issued = true;
}

void BoardingPass::revoke()
{
    if (!issued) {
        throw std::runtime_error(
            "Boarding pass is not issued"
        );
    }

    issued = false;
}

void BoardingPass::print() const
{
    if (!issued) {
        throw std::runtime_error(
            "Cannot print boarding pass before it is issued"
        );
    }

    const auto& passenger =
        booking->getPassenger();

    const auto& flight =
        booking->getFlight();

    const auto& seat =
        booking->getSeat();

    std::cout
        << "\n"
        << "====================================================\n"
        << "                    BOARDING PASS\n"
        << "====================================================\n";

    std::cout
        << "Boarding Pass:  "
        << boardingPassNumber
        << '\n';

    std::cout
        << "Booking Code:   "
        << booking->getBookingCode()
        << '\n';

    std::cout
        << "----------------------------------------------------\n";

    std::cout
        << "Passenger:      "
        << passenger->getFullName()
        << '\n';

    std::cout
        << "Passport:       "
        << passenger->getPassport().getPassportNumber()
        << '\n';

    std::cout
        << "----------------------------------------------------\n";

    std::cout
        << "Flight:         "
        << flight->getFlightNumber()
        << '\n';

    std::cout
        << "Airline:        "
        << flight->getAirlineName()
        << '\n';

    std::cout
        << "Route:          "
        << flight->getRoute()
        << '\n';

    std::cout
        << "Date:           "
        << flight->getDepartureDate()
        << '\n';

    std::cout
        << "Departure:      "
        << flight->getDepartureTime()
        << '\n';

    std::cout
        << "Boarding:       "
        << boardingTime
        << '\n';

    std::cout
        << "Gate:           "
        << flight->getGate()
        << '\n';

    std::cout
        << "----------------------------------------------------\n";

    std::cout
        << "Seat:           "
        << seat->getSeatNumber()
        << '\n';

    std::cout
        << "Class:          "
        << seat->getSeatClassString()
        << '\n';

    std::cout
        << "Boarding Group: "
        << boardingGroup
        << '\n';

    std::cout
        << "====================================================\n";
}