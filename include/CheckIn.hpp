#pragma once

#include "BoardingPass.hpp"
#include "Booking.hpp"
#include "Luggage.hpp"
#include "SeatMap.hpp"

#include <memory>
#include <string>

class CheckIn {
private:
    std::shared_ptr<Booking> booking;
    std::shared_ptr<SeatMap> seatMap;

    std::shared_ptr<BoardingPass> boardingPass;

    bool completed;

public:
    CheckIn(
        std::shared_ptr<Booking> booking,
        std::shared_ptr<SeatMap> seatMap
    );

    const std::shared_ptr<Booking>&
    getBooking() const;

    const std::shared_ptr<SeatMap>&
    getSeatMap() const;

    const std::shared_ptr<BoardingPass>&
    getBoardingPass() const;

    bool isCompleted() const;

    void selectSeat(
        const std::string& seatNumber
    );

    void changeSeat(
        const std::string& seatNumber
    );

    void addLuggage(
        std::shared_ptr<Luggage> luggage
    );

    double getExtraLuggageFee() const;

    void completeCheckIn(
        const std::string& boardingPassNumber,
        const std::string& boardingTime,
        int boardingGroup
    );

    void printSummary() const;
};