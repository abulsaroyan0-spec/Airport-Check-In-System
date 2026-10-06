#pragma once

#include <string>

enum class SeatClass {
    Economy,
    PremiumEconomy,
    Business,
    FirstClass
};

class Seat {
private:
    std::string seatNumber;
    SeatClass seatClass;
    bool occupied;

public:
    Seat(
        const std::string& seatNumber,
        SeatClass seatClass
    );

    const std::string& getSeatNumber() const;

    SeatClass getSeatClass() const;

    bool isOccupied() const;
    bool isAvailable() const;

    void occupy();
    void release();

    std::string getSeatClassString() const;

    void print() const;
};