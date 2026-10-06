#pragma once

#include "Seat.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class SeatMap {
private:
    std::size_t rowCount;
    std::size_t seatsPerRow;

    std::size_t firstClassRows;
    std::size_t businessRows;
    std::size_t premiumEconomyRows;

    std::vector<std::shared_ptr<Seat>> seats;

    SeatClass determineSeatClass(std::size_t row) const;

public:
    SeatMap(
        std::size_t rowCount,
        std::size_t seatsPerRow,
        std::size_t firstClassRows,
        std::size_t businessRows,
        std::size_t premiumEconomyRows
    );

    std::size_t getRowCount() const;
    std::size_t getSeatsPerRow() const;
    std::size_t getTotalSeatCount() const;

    std::size_t getOccupiedSeatCount() const;
    std::size_t getAvailableSeatCount() const;

    std::shared_ptr<Seat> findSeat(
        const std::string& seatNumber
    ) const;

    bool seatExists(
        const std::string& seatNumber
    ) const;

    bool isSeatAvailable(
        const std::string& seatNumber
    ) const;

    void reserveSeat(
        const std::string& seatNumber
    );

    void releaseSeat(
        const std::string& seatNumber
    );

    std::vector<std::shared_ptr<Seat>>
    getAvailableSeats() const;

    std::vector<std::shared_ptr<Seat>>
    getAvailableSeatsByClass(
        SeatClass seatClass
    ) const;

    void printSeatMap() const;
};