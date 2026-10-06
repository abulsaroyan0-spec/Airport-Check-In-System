#include "CheckIn.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

CheckIn::CheckIn(
    std::shared_ptr<Booking> booking,
    std::shared_ptr<SeatMap> seatMap
)
    : booking(std::move(booking)),
      seatMap(std::move(seatMap)),
      boardingPass(nullptr),
      completed(false)
{
    if (!this->booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    if (!this->seatMap) {
        throw std::invalid_argument(
            "Seat map cannot be null"
        );
    }

    if (this->booking->getStatus() ==
        BookingStatus::Cancelled) {

        throw std::runtime_error(
            "Cannot create check-in for cancelled booking"
        );
    }

    if (this->booking->getStatus() ==
        BookingStatus::Completed) {

        throw std::runtime_error(
            "Cannot create check-in for completed booking"
        );
    }
}

const std::shared_ptr<Booking>&
CheckIn::getBooking() const
{
    return booking;
}

const std::shared_ptr<SeatMap>&
CheckIn::getSeatMap() const
{
    return seatMap;
}

const std::shared_ptr<BoardingPass>&
CheckIn::getBoardingPass() const
{
    return boardingPass;
}

bool CheckIn::isCompleted() const
{
    return completed;
}

void CheckIn::selectSeat(
    const std::string& seatNumber
)
{
    if (completed) {
        throw std::runtime_error(
            "Cannot select seat after check-in is completed"
        );
    }

    if (seatNumber.empty()) {
        throw std::invalid_argument(
            "Seat number cannot be empty"
        );
    }

    if (booking->hasSeat()) {
        throw std::runtime_error(
            "Passenger already has a seat"
        );
    }

    std::shared_ptr<Seat> seat =
        seatMap->findSeat(seatNumber);

    if (!seat->isAvailable()) {
        throw std::runtime_error(
            "Selected seat is already occupied"
        );
    }

    booking->assignSeat(seat);
}

void CheckIn::changeSeat(
    const std::string& seatNumber
)
{
    if (completed) {
        throw std::runtime_error(
            "Cannot change seat after check-in is completed"
        );
    }

    if (seatNumber.empty()) {
        throw std::invalid_argument(
            "Seat number cannot be empty"
        );
    }

    if (!booking->hasSeat()) {
        throw std::runtime_error(
            "Passenger does not have a seat"
        );
    }

    const std::string currentSeatNumber =
        booking->getSeat()->getSeatNumber();

    if (currentSeatNumber == seatNumber) {
        throw std::runtime_error(
            "Passenger is already assigned to this seat"
        );
    }

    std::shared_ptr<Seat> newSeat =
        seatMap->findSeat(seatNumber);

    if (!newSeat->isAvailable()) {
        throw std::runtime_error(
            "Selected seat is already occupied"
        );
    }

    booking->removeSeat();
    booking->assignSeat(newSeat);
}

void CheckIn::addLuggage(
    std::shared_ptr<Luggage> luggage
)
{
    if (completed) {
        throw std::runtime_error(
            "Cannot add luggage after check-in is completed"
        );
    }

    if (!luggage) {
        throw std::invalid_argument(
            "Luggage cannot be null"
        );
    }

    booking->addLuggage(
        std::move(luggage)
    );
}

double CheckIn::getExtraLuggageFee() const
{
    return booking->getTotalExtraLuggageFee();
}

void CheckIn::completeCheckIn(
    const std::string& boardingPassNumber,
    const std::string& boardingTime,
    int boardingGroup
)
{
    if (completed) {
        throw std::runtime_error(
            "Check-in is already completed"
        );
    }

    if (!booking->hasSeat()) {
        throw std::runtime_error(
            "Passenger must select a seat before check-in"
        );
    }

    if (boardingPassNumber.empty()) {
        throw std::invalid_argument(
            "Boarding pass number cannot be empty"
        );
    }

    if (boardingTime.empty()) {
        throw std::invalid_argument(
            "Boarding time cannot be empty"
        );
    }

    if (boardingGroup <= 0) {
        throw std::invalid_argument(
            "Boarding group must be greater than zero"
        );
    }

    booking->checkIn();

    try {
        boardingPass =
            std::make_shared<BoardingPass>(
                boardingPassNumber,
                booking,
                boardingTime,
                boardingGroup
            );

        boardingPass->issue();
    }
    catch (...) {
        boardingPass.reset();
        throw;
    }

    completed = true;
}

void CheckIn::printSummary() const
{
    std::cout
        << "\n========================================\n"
        << "             CHECK-IN SUMMARY\n"
        << "========================================\n";

    std::cout
        << "Booking Code: "
        << booking->getBookingCode()
        << '\n';

    std::cout
        << "Passenger: "
        << booking->getPassenger()->getFullName()
        << '\n';

    std::cout
        << "Flight: "
        << booking->getFlight()->getFlightNumber()
        << '\n';

    std::cout
        << "Route: "
        << booking->getFlight()->getRoute()
        << '\n';

    std::cout
        << "Seat: ";

    if (booking->hasSeat()) {
        std::cout
            << booking->getSeat()->getSeatNumber();
    }
    else {
        std::cout
            << "Not selected";
    }

    std::cout << '\n';

    std::cout
        << "Luggage Count: "
        << booking->getLuggageCount()
        << '\n';

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Luggage Weight: "
        << booking->getTotalLuggageWeight()
        << " kg\n";

    std::cout
        << "Extra Fee: $"
        << getExtraLuggageFee()
        << '\n';

    std::cout
        << "Status: "
        << (completed
                ? "Check-In Completed"
                : "Check-In In Progress")
        << '\n';

    std::cout
        << "========================================\n";
}