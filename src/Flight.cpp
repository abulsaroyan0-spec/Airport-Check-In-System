#include "Flight.hpp"

#include <algorithm>
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

    if (!std::isdigit(static_cast<unsigned char>(time[0])) ||
        !std::isdigit(static_cast<unsigned char>(time[1])) ||
        !std::isdigit(static_cast<unsigned char>(time[3])) ||
        !std::isdigit(static_cast<unsigned char>(time[4]))) {
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

bool isValidFlightNumber(const std::string& flightNumber)
{
    if (flightNumber.size() < 2 || flightNumber.size() > 10) {
        return false;
    }

    return std::all_of(
        flightNumber.begin(),
        flightNumber.end(),
        [](unsigned char character) {
            return std::isalnum(character);
        }
    );
}

} // namespace

Flight::Flight(
    const std::string& flightNumber,
    const std::string& airlineName,
    std::shared_ptr<Airport> departureAirport,
    std::shared_ptr<Airport> arrivalAirport,
    const std::string& departureDate,
    const std::string& departureTime,
    const std::string& arrivalDate,
    const std::string& arrivalTime,
    const std::string& gate,
    std::size_t passengerCapacity
)
    : flightNumber(flightNumber),
      airlineName(airlineName),
      departureAirport(std::move(departureAirport)),
      arrivalAirport(std::move(arrivalAirport)),
      departureDate(departureDate),
      departureTime(departureTime),
      arrivalDate(arrivalDate),
      arrivalTime(arrivalTime),
      gate(gate),
      passengerCapacity(passengerCapacity),
      status(FlightStatus::Scheduled)
{
    if (!isValidFlightNumber(flightNumber)) {
        throw std::invalid_argument(
            "Invalid flight number"
        );
    }

    if (airlineName.empty()) {
        throw std::invalid_argument(
            "Airline name cannot be empty"
        );
    }

    if (!this->departureAirport) {
        throw std::invalid_argument(
            "Departure airport cannot be null"
        );
    }

    if (!this->arrivalAirport) {
        throw std::invalid_argument(
            "Arrival airport cannot be null"
        );
    }

    if (this->departureAirport->getCode() ==
        this->arrivalAirport->getCode()) {
        throw std::invalid_argument(
            "Departure and arrival airports cannot be the same"
        );
    }

    if (departureDate.empty()) {
        throw std::invalid_argument(
            "Departure date cannot be empty"
        );
    }

    if (!isValidTime(departureTime)) {
        throw std::invalid_argument(
            "Invalid departure time"
        );
    }

    if (arrivalDate.empty()) {
        throw std::invalid_argument(
            "Arrival date cannot be empty"
        );
    }

    if (!isValidTime(arrivalTime)) {
        throw std::invalid_argument(
            "Invalid arrival time"
        );
    }

    if (gate.empty()) {
        throw std::invalid_argument(
            "Gate cannot be empty"
        );
    }

    if (passengerCapacity == 0) {
        throw std::invalid_argument(
            "Passenger capacity must be greater than zero"
        );
    }
}

const std::string& Flight::getFlightNumber() const
{
    return flightNumber;
}

const std::string& Flight::getAirlineName() const
{
    return airlineName;
}

const std::shared_ptr<Airport>&
Flight::getDepartureAirport() const
{
    return departureAirport;
}

const std::shared_ptr<Airport>&
Flight::getArrivalAirport() const
{
    return arrivalAirport;
}

const std::string& Flight::getDepartureDate() const
{
    return departureDate;
}

const std::string& Flight::getDepartureTime() const
{
    return departureTime;
}

const std::string& Flight::getArrivalDate() const
{
    return arrivalDate;
}

const std::string& Flight::getArrivalTime() const
{
    return arrivalTime;
}

const std::string& Flight::getGate() const
{
    return gate;
}

std::size_t Flight::getPassengerCapacity() const
{
    return passengerCapacity;
}

FlightStatus Flight::getStatus() const
{
    return status;
}

void Flight::setDepartureDate(
    const std::string& departureDate
)
{
    if (departureDate.empty()) {
        throw std::invalid_argument(
            "Departure date cannot be empty"
        );
    }

    this->departureDate = departureDate;
}

void Flight::setDepartureTime(
    const std::string& departureTime
)
{
    if (!isValidTime(departureTime)) {
        throw std::invalid_argument(
            "Invalid departure time"
        );
    }

    this->departureTime = departureTime;
}

void Flight::setArrivalDate(
    const std::string& arrivalDate
)
{
    if (arrivalDate.empty()) {
        throw std::invalid_argument(
            "Arrival date cannot be empty"
        );
    }

    this->arrivalDate = arrivalDate;
}

void Flight::setArrivalTime(
    const std::string& arrivalTime
)
{
    if (!isValidTime(arrivalTime)) {
        throw std::invalid_argument(
            "Invalid arrival time"
        );
    }

    this->arrivalTime = arrivalTime;
}

void Flight::setGate(const std::string& gate)
{
    if (gate.empty()) {
        throw std::invalid_argument(
            "Gate cannot be empty"
        );
    }

    this->gate = gate;
}

void Flight::setStatus(FlightStatus status)
{
    this->status = status;
}

std::string Flight::getRoute() const
{
    return departureAirport->getCode() +
           " -> " +
           arrivalAirport->getCode();
}

std::string Flight::getStatusString() const
{
    switch (status) {
        case FlightStatus::Scheduled:
            return "Scheduled";

        case FlightStatus::Boarding:
            return "Boarding";

        case FlightStatus::Delayed:
            return "Delayed";

        case FlightStatus::Departed:
            return "Departed";

        case FlightStatus::Cancelled:
            return "Cancelled";

        case FlightStatus::Arrived:
            return "Arrived";
    }

    return "Unknown";
}

void Flight::print() const
{
    std::cout
        << "========================================\n"
        << "Flight Information\n"
        << "========================================\n"
        << "Flight Number: " << flightNumber << '\n'
        << "Airline: " << airlineName << '\n'
        << "Route: " << getRoute() << '\n'
        << "Departure: "
        << departureDate << ' '
        << departureTime << '\n'
        << "Arrival: "
        << arrivalDate << ' '
        << arrivalTime << '\n'
        << "Gate: " << gate << '\n'
        << "Capacity: " << passengerCapacity << '\n'
        << "Status: " << getStatusString() << '\n'
        << "========================================\n";
}