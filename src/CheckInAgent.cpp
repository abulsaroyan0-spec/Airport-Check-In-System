#include "CheckInAgent.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

CheckInAgent::CheckInAgent(
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
)
    : Employee(
          userId,
          username,
          password,
          employeeId,
          firstName,
          lastName,
          "Check-In",
          hireDate,
          salary
      ),
      agentCode(agentCode),
      counterNumber(counterNumber),
      processedCheckIns(0)
{
    if (agentCode.empty()) {
        throw std::invalid_argument(
            "Agent code cannot be empty"
        );
    }

    if (counterNumber.empty()) {
        throw std::invalid_argument(
            "Counter number cannot be empty"
        );
    }
}

const std::string&
CheckInAgent::getAgentCode() const
{
    return agentCode;
}

const std::string&
CheckInAgent::getCounterNumber() const
{
    return counterNumber;
}

std::size_t
CheckInAgent::getProcessedCheckIns() const
{
    return processedCheckIns;
}

void CheckInAgent::setCounterNumber(
    const std::string& counterNumber
)
{
    if (counterNumber.empty()) {
        throw std::invalid_argument(
            "Counter number cannot be empty"
        );
    }

    this->counterNumber = counterNumber;
}

std::shared_ptr<CheckIn>
CheckInAgent::createCheckIn(
    std::shared_ptr<Booking> booking,
    std::shared_ptr<SeatMap> seatMap
)
{
    if (!isActive()) {
        throw std::runtime_error(
            "Inactive agent cannot create check-in"
        );
    }

    if (!isOnDuty()) {
        throw std::runtime_error(
            "Agent must be on duty to create check-in"
        );
    }

    if (!booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    if (!seatMap) {
        throw std::invalid_argument(
            "Seat map cannot be null"
        );
    }

    return std::make_shared<CheckIn>(
        std::move(booking),
        std::move(seatMap)
    );
}

void CheckInAgent::completePassengerCheckIn(
    const std::shared_ptr<CheckIn>& checkIn,
    const std::string& boardingPassNumber,
    const std::string& boardingTime,
    int boardingGroup
)
{
    if (!isActive()) {
        throw std::runtime_error(
            "Inactive agent cannot complete check-in"
        );
    }

    if (!isOnDuty()) {
        throw std::runtime_error(
            "Agent must be on duty to complete check-in"
        );
    }

    if (!checkIn) {
        throw std::invalid_argument(
            "Check-in cannot be null"
        );
    }

    if (checkIn->isCompleted()) {
        throw std::runtime_error(
            "Check-in is already completed"
        );
    }

    checkIn->completeCheckIn(
        boardingPassNumber,
        boardingTime,
        boardingGroup
    );

    ++processedCheckIns;
}

std::string CheckInAgent::getRole() const
{
    return "Check-In Agent";
}

void CheckInAgent::print() const
{
    std::cout
        << "\n========================================\n"
        << "        CHECK-IN AGENT INFORMATION\n"
        << "========================================\n";

    std::cout
        << "Employee ID:     "
        << getEmployeeId()
        << '\n';

    std::cout
        << "Name:            "
        << getFullName()
        << '\n';

    std::cout
        << "Username:        "
        << getUsername()
        << '\n';

    std::cout
        << "Role:            "
        << getRole()
        << '\n';

    std::cout
        << "Agent Code:      "
        << agentCode
        << '\n';

    std::cout
        << "Counter:         "
        << counterNumber
        << '\n';

    std::cout
        << "Duty Status:     "
        << (isOnDuty()
                ? "On Duty"
                : "Off Duty")
        << '\n';

    std::cout
        << "Processed:       "
        << processedCheckIns
        << " check-ins\n";

    std::cout
        << "========================================\n";
}