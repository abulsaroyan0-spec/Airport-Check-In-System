#include "FileManager.hpp"

#include "Luggage.hpp"
#include "Seat.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct PendingBookingStatus {
    std::shared_ptr<Booking> booking;
    BookingStatus status;
};

FlightStatus intToFlightStatus(int value)
{
    switch (value) {
        case 0:
            return FlightStatus::Scheduled;

        case 1:
            return FlightStatus::Boarding;

        case 2:
            return FlightStatus::Delayed;

        case 3:
            return FlightStatus::Departed;

        case 4:
            return FlightStatus::Cancelled;

        case 5:
            return FlightStatus::Arrived;

        default:
            throw std::runtime_error(
                "Invalid flight status value"
            );
    }
}

BookingStatus intToBookingStatus(int value)
{
    switch (value) {
        case 0:
            return BookingStatus::Confirmed;

        case 1:
            return BookingStatus::CheckedIn;

        case 2:
            return BookingStatus::Cancelled;

        case 3:
            return BookingStatus::Completed;

        default:
            throw std::runtime_error(
                "Invalid booking status value"
            );
    }
}

SeatClass intToSeatClass(int value)
{
    switch (value) {
        case 0:
            return SeatClass::Economy;

        case 1:
            return SeatClass::PremiumEconomy;

        case 2:
            return SeatClass::Business;

        case 3:
            return SeatClass::FirstClass;

        default:
            throw std::runtime_error(
                "Invalid seat class value"
            );
    }
}

LuggageType intToLuggageType(int value)
{
    switch (value) {
        case 0:
            return LuggageType::Cabin;

        case 1:
            return LuggageType::Checked;

        case 2:
            return LuggageType::Oversized;

        case 3:
            return LuggageType::Special;

        default:
            throw std::runtime_error(
                "Invalid luggage type value"
            );
    }
}

void checkField(
    const std::string& value,
    const std::string& fieldName
)
{
    if (value.find('|') != std::string::npos) {
        throw std::runtime_error(
            fieldName +
            " cannot contain '|' character"
        );
    }
}

} // namespace

FileManager::FileManager(
    const std::string& dataDirectory
)
    : dataDirectory(dataDirectory)
{
    if (dataDirectory.empty()) {
        throw std::invalid_argument(
            "Data directory cannot be empty"
        );
    }

    std::filesystem::create_directories(
        dataDirectory
    );
}

const std::string&
FileManager::getDataDirectory() const
{
    return dataDirectory;
}

std::string
FileManager::getAirportsFilePath() const
{
    return dataDirectory + "/airports.txt";
}

std::string
FileManager::getPassengersFilePath() const
{
    return dataDirectory + "/passengers.txt";
}

std::string
FileManager::getFlightsFilePath() const
{
    return dataDirectory + "/flights.txt";
}

std::string
FileManager::getBookingsFilePath() const
{
    return dataDirectory + "/bookings.txt";
}

std::string
FileManager::getLuggageFilePath() const
{
    return dataDirectory + "/luggage.txt";
}

std::vector<std::string>
FileManager::split(
    const std::string& line,
    char delimiter
) const
{
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string part;

    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    if (!line.empty() &&
        line.back() == delimiter) {

        parts.emplace_back();
    }

    return parts;
}

void FileManager::saveAll(
    const Database& database
) const
{
    std::filesystem::create_directories(
        dataDirectory
    );

    {
        std::ofstream file(
            getAirportsFilePath(),
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open airports data file"
            );
        }

        for (const auto& airport :
             database.getAirports()) {

            checkField(
                airport->getCode(),
                "Airport code"
            );

            checkField(
                airport->getName(),
                "Airport name"
            );

            checkField(
                airport->getCity(),
                "Airport city"
            );

            checkField(
                airport->getCountry(),
                "Airport country"
            );

            file
                << airport->getCode()
                << '|'
                << airport->getName()
                << '|'
                << airport->getCity()
                << '|'
                << airport->getCountry()
                << '\n';
        }
    }

    {
        std::ofstream file(
            getPassengersFilePath(),
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open passengers data file"
            );
        }

        for (const auto& passenger :
             database.getPassengers()) {

            const Passport& passport =
                passenger->getPassport();

            checkField(
                passenger->getId(),
                "Passenger ID"
            );

            checkField(
                passenger->getFirstName(),
                "Passenger first name"
            );

            checkField(
                passenger->getLastName(),
                "Passenger last name"
            );

            file
                << passenger->getId()
                << '|'
                << passenger->getFirstName()
                << '|'
                << passenger->getLastName()
                << '|'
                << passenger->getDateOfBirth()
                << '|'
                << passport.getPassportNumber()
                << '|'
                << passport.getNationality()
                << '|'
                << passport.getExpiryDate()
                << '\n';
        }
    }

    {
        std::ofstream file(
            getFlightsFilePath(),
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open flights data file"
            );
        }

        for (const auto& flight :
             database.getFlights()) {

            file
                << flight->getFlightNumber()
                << '|'
                << flight->getAirlineName()
                << '|'
                << flight
                       ->getDepartureAirport()
                       ->getCode()
                << '|'
                << flight
                       ->getArrivalAirport()
                       ->getCode()
                << '|'
                << flight->getDepartureDate()
                << '|'
                << flight->getDepartureTime()
                << '|'
                << flight->getArrivalDate()
                << '|'
                << flight->getArrivalTime()
                << '|'
                << flight->getGate()
                << '|'
                << flight->getPassengerCapacity()
                << '|'
                << static_cast<int>(
                       flight->getStatus()
                   )
                << '\n';
        }
    }

    {
        std::ofstream file(
            getBookingsFilePath(),
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open bookings data file"
            );
        }

        for (const auto& booking :
             database.getBookings()) {

            std::string seatNumber;
            int seatClass = -1;

            if (booking->hasSeat()) {
                seatNumber =
                    booking
                        ->getSeat()
                        ->getSeatNumber();

                seatClass =
                    static_cast<int>(
                        booking
                            ->getSeat()
                            ->getSeatClass()
                    );
            }

            file
                << booking->getBookingCode()
                << '|'
                << booking
                       ->getPassenger()
                       ->getId()
                << '|'
                << booking
                       ->getFlight()
                       ->getFlightNumber()
                << '|'
                << static_cast<int>(
                       booking->getStatus()
                   )
                << '|'
                << seatNumber
                << '|'
                << seatClass
                << '\n';
        }
    }

    {
        std::ofstream file(
            getLuggageFilePath(),
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot open luggage data file"
            );
        }

        for (const auto& booking :
             database.getBookings()) {

            for (const auto& luggage :
                 booking->getLuggageList()) {

                file
                    << booking->getBookingCode()
                    << '|'
                    << luggage->getTagNumber()
                    << '|'
                    << luggage->getWeight()
                    << '|'
                    << luggage->getMaxAllowedWeight()
                    << '|'
                    << luggage->getExtraFeePerKg()
                    << '|'
                    << static_cast<int>(
                           luggage->getType()
                       )
                    << '\n';
            }
        }
    }
}

void FileManager::loadAll(
    Database& database
) const
{
    if (database.getAirportCount() != 0 ||
        database.getPassengerCount() != 0 ||
        database.getFlightCount() != 0 ||
        database.getBookingCount() != 0) {

        throw std::runtime_error(
            "Database must be empty before loading data"
        );
    }

    {
        std::ifstream file(
            getAirportsFilePath()
        );

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {

                if (line.empty()) {
                    continue;
                }

                const auto parts =
                    split(line, '|');

                if (parts.size() != 4) {
                    throw std::runtime_error(
                        "Invalid airport data line"
                    );
                }

                auto airport =
                    std::make_shared<Airport>(
                        parts[0],
                        parts[1],
                        parts[2],
                        parts[3]
                    );

                database.addAirport(
                    airport
                );
            }
        }
    }

    {
        std::ifstream file(
            getPassengersFilePath()
        );

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {

                if (line.empty()) {
                    continue;
                }

                const auto parts =
                    split(line, '|');

                if (parts.size() != 7) {
                    throw std::runtime_error(
                        "Invalid passenger data line"
                    );
                }

                Passport passport(
                    parts[4],
                    parts[5],
                    parts[6]
                );

                auto passenger =
                    std::make_shared<Passenger>(
                        parts[0],
                        parts[1],
                        parts[2],
                        parts[3],
                        passport
                    );

                database.addPassenger(
                    passenger
                );
            }
        }
    }

    {
        std::ifstream file(
            getFlightsFilePath()
        );

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {

                if (line.empty()) {
                    continue;
                }

                const auto parts =
                    split(line, '|');

                if (parts.size() != 11) {
                    throw std::runtime_error(
                        "Invalid flight data line"
                    );
                }

                auto departureAirport =
                    database.findAirport(
                        parts[2]
                    );

                auto arrivalAirport =
                    database.findAirport(
                        parts[3]
                    );

                const std::size_t capacity =
                    static_cast<std::size_t>(
                        std::stoull(parts[9])
                    );

                auto flight =
                    std::make_shared<Flight>(
                        parts[0],
                        parts[1],
                        departureAirport,
                        arrivalAirport,
                        parts[4],
                        parts[5],
                        parts[6],
                        parts[7],
                        parts[8],
                        capacity
                    );

                flight->setStatus(
                    intToFlightStatus(
                        std::stoi(parts[10])
                    )
                );

                database.addFlight(
                    flight
                );
            }
        }
    }

    std::vector<PendingBookingStatus>
        pendingStatuses;

    {
        std::ifstream file(
            getBookingsFilePath()
        );

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {

                if (line.empty()) {
                    continue;
                }

                const auto parts =
                    split(line, '|');

                if (parts.size() != 6) {
                    throw std::runtime_error(
                        "Invalid booking data line"
                    );
                }

                auto passenger =
                    database.findPassenger(
                        parts[1]
                    );

                auto flight =
                    database.findFlight(
                        parts[2]
                    );

                auto booking =
                    std::make_shared<Booking>(
                        parts[0],
                        passenger,
                        flight
                    );

                if (!parts[4].empty()) {

                    const int seatClassValue =
                        std::stoi(parts[5]);

                    auto seat =
                        std::make_shared<Seat>(
                            parts[4],
                            intToSeatClass(
                                seatClassValue
                            )
                        );

                    booking->assignSeat(
                        seat
                    );
                }

                database.addBooking(
                    booking
                );

                PendingBookingStatus pending;

                pending.booking =
                    booking;

                pending.status =
                    intToBookingStatus(
                        std::stoi(parts[3])
                    );

                pendingStatuses.push_back(
                    pending
                );
            }
        }
    }

    {
        std::ifstream file(
            getLuggageFilePath()
        );

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {

                if (line.empty()) {
                    continue;
                }

                const auto parts =
                    split(line, '|');

                if (parts.size() != 6) {
                    throw std::runtime_error(
                        "Invalid luggage data line"
                    );
                }

                auto booking =
                    database.findBooking(
                        parts[0]
                    );

                auto luggage =
                    std::make_shared<Luggage>(
                        parts[1],
                        std::stod(parts[2]),
                        std::stod(parts[3]),
                        std::stod(parts[4]),
                        intToLuggageType(
                            std::stoi(parts[5])
                        )
                    );

                booking->addLuggage(
                    luggage
                );
            }
        }
    }

    for (const auto& pending :
         pendingStatuses) {

        switch (pending.status) {

            case BookingStatus::Confirmed:
                break;

            case BookingStatus::CheckedIn:

                if (!pending.booking->hasSeat()) {
                    throw std::runtime_error(
                        "Checked-in booking has no seat"
                    );
                }

                pending.booking->checkIn();

                break;

            case BookingStatus::Cancelled:

                pending.booking->cancel();

                break;

            case BookingStatus::Completed:

                if (!pending.booking->hasSeat()) {
                    throw std::runtime_error(
                        "Completed booking has no seat"
                    );
                }

                pending.booking->checkIn();
                pending.booking->complete();

                break;
        }
    }
}

void FileManager::clearDataFiles() const
{
    std::filesystem::create_directories(
        dataDirectory
    );

    const std::vector<std::string> files = {
        getAirportsFilePath(),
        getPassengersFilePath(),
        getFlightsFilePath(),
        getBookingsFilePath(),
        getLuggageFilePath()
    };

    for (const auto& path : files) {

        std::ofstream file(
            path,
            std::ios::trunc
        );

        if (!file.is_open()) {
            throw std::runtime_error(
                "Cannot clear data file: " +
                path
            );
        }
    }
}