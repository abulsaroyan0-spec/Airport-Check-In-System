#pragma once

#include "CheckIn.hpp"
#include "Employee.hpp"

#include <cstddef>
#include <memory>
#include <string>

class CheckInAgent : public Employee {
private:
    std::string agentCode;
    std::string counterNumber;

    std::size_t processedCheckIns;

public:
    CheckInAgent(
        const std::string& userId,
        const std::string& username,
        const std::string& password,
        const std::string& employeeId,
        const std::string& firstName,
        const std::string& lastName,
        const std::string& hireDate,
        double salary,
        const std::string& agentCode,
        const std::string& counterNumber
    );

    const std::string& getAgentCode() const;
    const std::string& getCounterNumber() const;

    std::size_t getProcessedCheckIns() const;

    void setCounterNumber(
        const std::string& counterNumber
    );

    std::shared_ptr<CheckIn> createCheckIn(
        std::shared_ptr<Booking> booking,
        std::shared_ptr<SeatMap> seatMap
    );

    void completePassengerCheckIn(
        const std::shared_ptr<CheckIn>& checkIn,
        const std::string& boardingPassNumber,
        const std::string& boardingTime,
        int boardingGroup
    );

    std::string getRole() const override;

    void print() const override;
};