#include "SeatMap.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

SeatMap::SeatMap(
    std::size_t rowCount,
    std::size_t seatsPerRow,
    std::size_t firstClassRows,
    std::size_t businessRows,
    std::size_t premiumEconomyRows
)
    : rowCount(rowCount),
      seatsPerRow(seatsPerRow),
      firstClassRows(firstClassRows),
      businessRows(businessRows),
      premiumEconomyRows(premiumEconomyRows)
{
    if (rowCount == 0) {
        throw std::invalid_argument(
            "Row count must be greater than zero"
        );
    }

    if (seatsPerRow == 0) {
        throw std::invalid_argument(
            "Seats per row must be greater than zero"
        );
    }

    if (seatsPerRow > 26) {
        throw std::invalid_argument(
            "Seats per row cannot exceed 26"
        );
    }

    const std::size_t specialRows =
        firstClassRows +
        businessRows +
        premiumEconomyRows;

    if (specialRows > rowCount) {
        throw std::invalid_argument(
            "Seat class rows exceed total row count"
        );
    }

    seats.reserve(
        rowCount * seatsPerRow
    );

    for (std::size_t row = 1;
         row <= rowCount;
         ++row) {

        const SeatClass seatClass =
            determineSeatClass(row);

        for (std::size_t column = 0;
             column < seatsPerRow;
             ++column) {

            const char seatLetter =
                static_cast<char>('A' + column);

            const std::string seatNumber =
                std::to_string(row) +
                seatLetter;

            seats.push_back(
                std::make_shared<Seat>(
                    seatNumber,
                    seatClass
                )
            );
        }
    }
}

SeatClass SeatMap::determineSeatClass(
    std::size_t row
) const
{
    if (row <= firstClassRows) {
        return SeatClass::FirstClass;
    }

    if (row <=
        firstClassRows +
        businessRows) {

        return SeatClass::Business;
    }

    if (row <=
        firstClassRows +
        businessRows +
        premiumEconomyRows) {

        return SeatClass::PremiumEconomy;
    }

    return SeatClass::Economy;
}

std::size_t SeatMap::getRowCount() const
{
    return rowCount;
}

std::size_t SeatMap::getSeatsPerRow() const
{
    return seatsPerRow;
}

std::size_t SeatMap::getTotalSeatCount() const
{
    return seats.size();
}

std::size_t SeatMap::getOccupiedSeatCount() const
{
    std::size_t count = 0;

    for (const auto& seat : seats) {
        if (seat->isOccupied()) {
            ++count;
        }
    }

    return count;
}

std::size_t SeatMap::getAvailableSeatCount() const
{
    return getTotalSeatCount()
           - getOccupiedSeatCount();
}

std::shared_ptr<Seat> SeatMap::findSeat(
    const std::string& seatNumber
) const
{
    for (const auto& seat : seats) {

        if (seat->getSeatNumber()
            == seatNumber) {

            return seat;
        }
    }

    throw std::runtime_error(
        "Seat not found: " +
        seatNumber
    );
}

bool SeatMap::seatExists(
    const std::string& seatNumber
) const
{
    for (const auto& seat : seats) {

        if (seat->getSeatNumber()
            == seatNumber) {

            return true;
        }
    }

    return false;
}

bool SeatMap::isSeatAvailable(
    const std::string& seatNumber
) const
{
    return findSeat(
        seatNumber
    )->isAvailable();
}

void SeatMap::reserveSeat(
    const std::string& seatNumber
)
{
    std::shared_ptr<Seat> seat =
        findSeat(seatNumber);

    seat->occupy();
}

void SeatMap::releaseSeat(
    const std::string& seatNumber
)
{
    std::shared_ptr<Seat> seat =
        findSeat(seatNumber);

    seat->release();
}

std::vector<std::shared_ptr<Seat>>
SeatMap::getAvailableSeats() const
{
    std::vector<
        std::shared_ptr<Seat>
    > availableSeats;

    for (const auto& seat : seats) {

        if (seat->isAvailable()) {
            availableSeats.push_back(
                seat
            );
        }
    }

    return availableSeats;
}

std::vector<std::shared_ptr<Seat>>
SeatMap::getAvailableSeatsByClass(
    SeatClass seatClass
) const
{
    std::vector<
        std::shared_ptr<Seat>
    > availableSeats;

    for (const auto& seat : seats) {

        if (seat->isAvailable() &&
            seat->getSeatClass()
                == seatClass) {

            availableSeats.push_back(
                seat
            );
        }
    }

    return availableSeats;
}

void SeatMap::printSeatMap() const
{
    std::cout
        << "\n========================================\n"
        << "              SEAT MAP\n"
        << "========================================\n\n";

    std::cout << "     ";

    for (std::size_t column = 0;
         column < seatsPerRow;
         ++column) {

        const char seatLetter =
            static_cast<char>('A' + column);

        std::cout
            << std::setw(6)
            << seatLetter;

        if (seatsPerRow >= 4 &&
            column + 1 == seatsPerRow / 2) {

            std::cout << "   ";
        }
    }

    std::cout << "\n\n";

    for (std::size_t row = 1;
         row <= rowCount;
         ++row) {

        std::cout
            << std::setw(3)
            << row
            << "  ";

        for (std::size_t column = 0;
             column < seatsPerRow;
             ++column) {

            const char seatLetter =
                static_cast<char>('A' + column);

            const std::string seatNumber =
                std::to_string(row)
                + seatLetter;

            const auto seat =
                findSeat(seatNumber);

            if (seat->isOccupied()) {
                std::cout
                    << std::setw(6)
                    << "[X]";
            } else {
                std::cout
                    << std::setw(6)
                    << "[ ]";
            }

            if (seatsPerRow >= 4 &&
                column + 1 ==
                    seatsPerRow / 2) {

                std::cout << "   ";
            }
        }

        std::cout << '\n';
    }

    std::cout
        << "\n----------------------------------------\n"
        << "[ ] = Available\n"
        << "[X] = Occupied\n"
        << "----------------------------------------\n"
        << "Total seats: "
        << getTotalSeatCount()
        << '\n'
        << "Available: "
        << getAvailableSeatCount()
        << '\n'
        << "Occupied: "
        << getOccupiedSeatCount()
        << '\n'
        << "========================================\n";
}