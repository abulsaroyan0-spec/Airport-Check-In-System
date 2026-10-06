#pragma once

#include "Airport.hpp"
#include "Booking.hpp"
#include "Employee.hpp"
#include "Flight.hpp"
#include "Passenger.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Database {
private:
    std::vector<std::shared_ptr<Airport>> airports;
    std::vector<std::shared_ptr<Passenger>> passengers;
    std::vector<std::shared_ptr<Flight>> flights;
    std::vector<std::shared_ptr<Booking>> bookings;
    std::vector<std::shared_ptr<Employee>> employees;

public:
    Database() = default;

    void addAirport(
        std::shared_ptr<Airport> airport
    );

    void addPassenger(
        std::shared_ptr<Passenger> passenger
    );

    void addFlight(
        std::shared_ptr<Flight> flight
    );

    void addBooking(
        std::shared_ptr<Booking> booking
    );

    void addEmployee(
        std::shared_ptr<Employee> employee
    );

    std::shared_ptr<Airport> findAirport(
        const std::string& code
    ) const;

    std::shared_ptr<Passenger> findPassenger(
        const std::string& passengerId
    ) const;

    std::shared_ptr<Passenger> findPassengerByPassport(
        const std::string& passportNumber
    ) const;

    std::shared_ptr<Flight> findFlight(
        const std::string& flightNumber
    ) const;

    std::shared_ptr<Booking> findBooking(
        const std::string& bookingCode
    ) const;

    std::shared_ptr<Employee> findEmployee(
        const std::string& employeeId
    ) const;

    void removeAirport(
        const std::string& code
    );

    void removePassenger(
        const std::string& passengerId
    );

    void removeFlight(
        const std::string& flightNumber
    );

    void removeBooking(
        const std::string& bookingCode
    );

    void removeEmployee(
        const std::string& employeeId
    );

    const std::vector<std::shared_ptr<Airport>>&
    getAirports() const;

    const std::vector<std::shared_ptr<Passenger>>&
    getPassengers() const;

    const std::vector<std::shared_ptr<Flight>>&
    getFlights() const;

    const std::vector<std::shared_ptr<Booking>>&
    getBookings() const;

    const std::vector<std::shared_ptr<Employee>>&
    getEmployees() const;

    std::size_t getAirportCount() const;
    std::size_t getPassengerCount() const;
    std::size_t getFlightCount() const;
    std::size_t getBookingCount() const;
    std::size_t getEmployeeCount() const;

    void printSummary() const;
};