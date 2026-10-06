#include "Database.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

void Database::addAirport(
    std::shared_ptr<Airport> airport
)
{
    if (!airport) {
        throw std::invalid_argument(
            "Airport cannot be null"
        );
    }

    for (const auto& existingAirport : airports) {
        if (existingAirport->getCode() ==
            airport->getCode()) {

            throw std::runtime_error(
                "Airport already exists: " +
                airport->getCode()
            );
        }
    }

    airports.push_back(
        std::move(airport)
    );
}

void Database::addPassenger(
    std::shared_ptr<Passenger> passenger
)
{
    if (!passenger) {
        throw std::invalid_argument(
            "Passenger cannot be null"
        );
    }

    for (const auto& existingPassenger : passengers) {
        if (existingPassenger->getId() ==
            passenger->getId()) {

            throw std::runtime_error(
                "Passenger ID already exists: " +
                passenger->getId()
            );
        }

        if (existingPassenger
                ->getPassport()
                .getPassportNumber() ==
            passenger
                ->getPassport()
                .getPassportNumber()) {

            throw std::runtime_error(
                "Passport number already exists: " +
                passenger
                    ->getPassport()
                    .getPassportNumber()
            );
        }
    }

    passengers.push_back(
        std::move(passenger)
    );
}

void Database::addFlight(
    std::shared_ptr<Flight> flight
)
{
    if (!flight) {
        throw std::invalid_argument(
            "Flight cannot be null"
        );
    }

    for (const auto& existingFlight : flights) {
        if (existingFlight->getFlightNumber() ==
            flight->getFlightNumber()) {

            throw std::runtime_error(
                "Flight already exists: " +
                flight->getFlightNumber()
            );
        }
    }

    flights.push_back(
        std::move(flight)
    );
}

void Database::addBooking(
    std::shared_ptr<Booking> booking
)
{
    if (!booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    for (const auto& existingBooking : bookings) {
        if (existingBooking->getBookingCode() ==
            booking->getBookingCode()) {

            throw std::runtime_error(
                "Booking already exists: " +
                booking->getBookingCode()
            );
        }
    }

    bookings.push_back(
        std::move(booking)
    );
}

void Database::addEmployee(
    std::shared_ptr<Employee> employee
)
{
    if (!employee) {
        throw std::invalid_argument(
            "Employee cannot be null"
        );
    }

    for (const auto& existingEmployee : employees) {

        if (existingEmployee->getEmployeeId() ==
            employee->getEmployeeId()) {

            throw std::runtime_error(
                "Employee ID already exists: " +
                employee->getEmployeeId()
            );
        }

        if (existingEmployee->getUsername() ==
            employee->getUsername()) {

            throw std::runtime_error(
                "Username already exists: " +
                employee->getUsername()
            );
        }
    }

    employees.push_back(
        std::move(employee)
    );
}

std::shared_ptr<Airport>
Database::findAirport(
    const std::string& code
) const
{
    for (const auto& airport : airports) {

        if (airport->getCode() == code) {
            return airport;
        }
    }

    throw std::runtime_error(
        "Airport not found: " + code
    );
}

std::shared_ptr<Passenger>
Database::findPassenger(
    const std::string& passengerId
) const
{
    for (const auto& passenger : passengers) {

        if (passenger->getId() ==
            passengerId) {

            return passenger;
        }
    }

    throw std::runtime_error(
        "Passenger not found: " +
        passengerId
    );
}

std::shared_ptr<Passenger>
Database::findPassengerByPassport(
    const std::string& passportNumber
) const
{
    for (const auto& passenger : passengers) {

        if (passenger
                ->getPassport()
                .getPassportNumber() ==
            passportNumber) {

            return passenger;
        }
    }

    throw std::runtime_error(
        "Passenger with passport not found: " +
        passportNumber
    );
}

std::shared_ptr<Flight>
Database::findFlight(
    const std::string& flightNumber
) const
{
    for (const auto& flight : flights) {

        if (flight->getFlightNumber() ==
            flightNumber) {

            return flight;
        }
    }

    throw std::runtime_error(
        "Flight not found: " +
        flightNumber
    );
}

std::shared_ptr<Booking>
Database::findBooking(
    const std::string& bookingCode
) const
{
    for (const auto& booking : bookings) {

        if (booking->getBookingCode() ==
            bookingCode) {

            return booking;
        }
    }

    throw std::runtime_error(
        "Booking not found: " +
        bookingCode
    );
}

std::shared_ptr<Employee>
Database::findEmployee(
    const std::string& employeeId
) const
{
    for (const auto& employee : employees) {

        if (employee->getEmployeeId() ==
            employeeId) {

            return employee;
        }
    }

    throw std::runtime_error(
        "Employee not found: " +
        employeeId
    );
}

void Database::removeAirport(
    const std::string& code
)
{
    for (const auto& flight : flights) {

        if (flight
                ->getDepartureAirport()
                ->getCode() == code ||
            flight
                ->getArrivalAirport()
                ->getCode() == code) {

            throw std::runtime_error(
                "Cannot remove airport because it is used by a flight"
            );
        }
    }

    for (auto iterator = airports.begin();
         iterator != airports.end();
         ++iterator) {

        if ((*iterator)->getCode() == code) {
            airports.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Airport not found: " + code
    );
}

void Database::removePassenger(
    const std::string& passengerId
)
{
    for (const auto& booking : bookings) {

        if (booking
                ->getPassenger()
                ->getId() ==
            passengerId) {

            throw std::runtime_error(
                "Cannot remove passenger because a booking exists"
            );
        }
    }

    for (auto iterator = passengers.begin();
         iterator != passengers.end();
         ++iterator) {

        if ((*iterator)->getId() ==
            passengerId) {

            passengers.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Passenger not found: " +
        passengerId
    );
}

void Database::removeFlight(
    const std::string& flightNumber
)
{
    for (const auto& booking : bookings) {

        if (booking
                ->getFlight()
                ->getFlightNumber() ==
            flightNumber) {

            throw std::runtime_error(
                "Cannot remove flight because bookings exist"
            );
        }
    }

    for (auto iterator = flights.begin();
         iterator != flights.end();
         ++iterator) {

        if ((*iterator)->getFlightNumber() ==
            flightNumber) {

            flights.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Flight not found: " +
        flightNumber
    );
}

void Database::removeBooking(
    const std::string& bookingCode
)
{
    for (auto iterator = bookings.begin();
         iterator != bookings.end();
         ++iterator) {

        if ((*iterator)->getBookingCode() ==
            bookingCode) {

            if ((*iterator)->getStatus() ==
                BookingStatus::CheckedIn) {

                throw std::runtime_error(
                    "Cannot remove a checked-in booking"
                );
            }

            if ((*iterator)->getStatus() ==
                BookingStatus::Completed) {

                throw std::runtime_error(
                    "Cannot remove a completed booking"
                );
            }

            if ((*iterator)->hasSeat()) {
                (*iterator)->removeSeat();
            }

            bookings.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Booking not found: " +
        bookingCode
    );
}

void Database::removeEmployee(
    const std::string& employeeId
)
{
    for (auto iterator = employees.begin();
         iterator != employees.end();
         ++iterator) {

        if ((*iterator)->getEmployeeId() ==
            employeeId) {

            if ((*iterator)->isOnDuty()) {
                throw std::runtime_error(
                    "Cannot remove employee while on duty"
                );
            }

            employees.erase(iterator);
            return;
        }
    }

    throw std::runtime_error(
        "Employee not found: " +
        employeeId
    );
}

const std::vector<std::shared_ptr<Airport>>&
Database::getAirports() const
{
    return airports;
}

const std::vector<std::shared_ptr<Passenger>>&
Database::getPassengers() const
{
    return passengers;
}

const std::vector<std::shared_ptr<Flight>>&
Database::getFlights() const
{
    return flights;
}

const std::vector<std::shared_ptr<Booking>>&
Database::getBookings() const
{
    return bookings;
}

const std::vector<std::shared_ptr<Employee>>&
Database::getEmployees() const
{
    return employees;
}

std::size_t Database::getAirportCount() const
{
    return airports.size();
}

std::size_t Database::getPassengerCount() const
{
    return passengers.size();
}

std::size_t Database::getFlightCount() const
{
    return flights.size();
}

std::size_t Database::getBookingCount() const
{
    return bookings.size();
}

std::size_t Database::getEmployeeCount() const
{
    return employees.size();
}

void Database::printSummary() const
{
    std::cout
        << "\n========================================\n"
        << "           DATABASE SUMMARY\n"
        << "========================================\n"
        << "Airports:   "
        << getAirportCount()
        << '\n'
        << "Passengers: "
        << getPassengerCount()
        << '\n'
        << "Flights:    "
        << getFlightCount()
        << '\n'
        << "Bookings:   "
        << getBookingCount()
        << '\n'
        << "Employees:  "
        << getEmployeeCount()
        << '\n'
        << "========================================\n";
}