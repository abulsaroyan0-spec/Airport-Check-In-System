#pragma once

#include "Airport.hpp"

#include <cstddef>
#include <memory>
#include <string>

enum class FlightStatus {
    Scheduled,
    Boarding,
    Delayed,
    Departed,
    Cancelled,
    Arrived
};

class Flight {
private:
    std::string flightNumber;
    std::string airlineName;

    std::shared_ptr<Airport> departureAirport;
    std::shared_ptr<Airport> arrivalAirport;

    std::string departureDate;
    std::string departureTime;
    std::string arrivalDate;
    std::string arrivalTime;

    std::string gate;

    std::size_t passengerCapacity;
    FlightStatus status;

public:
    Flight(
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
    );

    const std::string& getFlightNumber() const;
    const std::string& getAirlineName() const;

    const std::shared_ptr<Airport>& getDepartureAirport() const;
    const std::shared_ptr<Airport>& getArrivalAirport() const;

    const std::string& getDepartureDate() const;
    const std::string& getDepartureTime() const;

    const std::string& getArrivalDate() const;
    const std::string& getArrivalTime() const;

    const std::string& getGate() const;

    std::size_t getPassengerCapacity() const;

    FlightStatus getStatus() const;

    void setDepartureDate(const std::string& departureDate);
    void setDepartureTime(const std::string& departureTime);

    void setArrivalDate(const std::string& arrivalDate);
    void setArrivalTime(const std::string& arrivalTime);

    void setGate(const std::string& gate);
    void setStatus(FlightStatus status);

    std::string getRoute() const;
    std::string getStatusString() const;

    void print() const;
};