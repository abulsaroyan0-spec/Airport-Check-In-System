#pragma once

#include "Booking.hpp"

#include <memory>
#include <string>

class BoardingPass {
private:
    std::string boardingPassNumber;
    std::shared_ptr<Booking> booking;

    std::string boardingTime;
    int boardingGroup;

    bool issued;

public:
    BoardingPass(
        const std::string& boardingPassNumber,
        std::shared_ptr<Booking> booking,
        const std::string& boardingTime,
        int boardingGroup
    );

    const std::string& getBoardingPassNumber() const;

    const std::shared_ptr<Booking>&
    getBooking() const;

    const std::string& getBoardingTime() const;

    int getBoardingGroup() const;

    bool isIssued() const;

    void setBoardingTime(
        const std::string& boardingTime
    );

    void setBoardingGroup(
        int boardingGroup
    );

    void issue();

    void revoke();

    void print() const;
};