#include "Application.hpp"

#include "Airport.hpp"
#include "Booking.hpp"
#include "CheckInAgent.hpp"
#include "Flight.hpp"
#include "Passenger.hpp"
#include "Passport.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

Application::Application()
    : database(),
      logger("data/airport.log"),
      fileManager("data"),
      menu(database, logger)
{
}

bool Application::hasSavedData() const
{
    const std::filesystem::path airportsFile =
        "data/airports.txt";

    const std::filesystem::path passengersFile =
        "data/passengers.txt";

    const std::filesystem::path flightsFile =
        "data/flights.txt";

    const std::filesystem::path bookingsFile =
        "data/bookings.txt";

    if (!std::filesystem::exists(airportsFile) ||
        !std::filesystem::exists(passengersFile) ||
        !std::filesystem::exists(flightsFile) ||
        !std::filesystem::exists(bookingsFile)) {

        return false;
    }

    if (std::filesystem::file_size(airportsFile) == 0 ||
        std::filesystem::file_size(passengersFile) == 0 ||
        std::filesystem::file_size(flightsFile) == 0) {

        return false;
    }

    return true;
}

void Application::initializeSampleData()
{
    logger.info(
        "Creating sample airport data"
    );

    auto yerevanAirport =
        std::make_shared<Airport>(
            "EVN",
            "Zvartnots International Airport",
            "Yerevan",
            "Armenia"
        );

    auto frankfurtAirport =
        std::make_shared<Airport>(
            "FRA",
            "Frankfurt Airport",
            "Frankfurt",
            "Germany"
        );

    auto parisAirport =
        std::make_shared<Airport>(
            "CDG",
            "Charles de Gaulle Airport",
            "Paris",
            "France"
        );

    auto dubaiAirport =
        std::make_shared<Airport>(
            "DXB",
            "Dubai International Airport",
            "Dubai",
            "United Arab Emirates"
        );

    database.addAirport(
        yerevanAirport
    );

    database.addAirport(
        frankfurtAirport
    );

    database.addAirport(
        parisAirport
    );

    database.addAirport(
        dubaiAirport
    );

    Passport passport1(
        "AM0584217",
        "Armenian",
        "2031-08-15"
    );

    Passport passport2(
        "AM0736421",
        "Armenian",
        "2030-04-21"
    );

    Passport passport3(
        "AM0912458",
        "Armenian",
        "2032-01-10"
    );

    auto passenger1 =
        std::make_shared<Passenger>(
            "P001",
            "Albert",
            "Saroyan",
            "2003-06-14",
            passport1
        );

    auto passenger2 =
        std::make_shared<Passenger>(
            "P002",
            "Arman",
            "Petrosyan",
            "1997-11-03",
            passport2
        );

    auto passenger3 =
        std::make_shared<Passenger>(
            "P003",
            "Mariam",
            "Hakobyan",
            "2000-02-17",
            passport3
        );

    database.addPassenger(
        passenger1
    );

    database.addPassenger(
        passenger2
    );

    database.addPassenger(
        passenger3
    );

    auto flight1 =
        std::make_shared<Flight>(
            "LH1561",
            "Lufthansa",
            yerevanAirport,
            frankfurtAirport,
            "2026-10-15",
            "05:30",
            "2026-10-15",
            "08:20",
            "A12",
            180
        );

    auto flight2 =
        std::make_shared<Flight>(
            "AF1061",
            "AirFrance",
            yerevanAirport,
            parisAirport,
            "2026-10-16",
            "09:40",
            "2026-10-16",
            "13:10",
            "B07",
            180
        );

    auto flight3 =
        std::make_shared<Flight>(
            "EK2207",
            "Emirates",
            yerevanAirport,
            dubaiAirport,
            "2026-10-17",
            "12:15",
            "2026-10-17",
            "15:20",
            "C03",
            240
        );

    flight1->setStatus(
        FlightStatus::Scheduled
    );

    flight2->setStatus(
        FlightStatus::Scheduled
    );

    flight3->setStatus(
        FlightStatus::Scheduled
    );

    database.addFlight(
        flight1
    );

    database.addFlight(
        flight2
    );

    database.addFlight(
        flight3
    );

    auto booking1 =
        std::make_shared<Booking>(
            "ABX492",
            passenger1,
            flight1
        );

    auto booking2 =
        std::make_shared<Booking>(
            "CDP731",
            passenger2,
            flight2
        );

    auto booking3 =
        std::make_shared<Booking>(
            "EKM829",
            passenger3,
            flight3
        );

    database.addBooking(
        booking1
    );

    database.addBooking(
        booking2
    );

    database.addBooking(
        booking3
    );

    auto checkInAgent =
        std::make_shared<CheckInAgent>(
            "USR001",
            "agent.alex",
            "Airport123",
            "EMP001",
            "Alex",
            "Morgan",
            "2024-03-10",
            2500.0,
            "AG001",
            "C12"
        );

    database.addEmployee(
        checkInAgent
    );

    logger.info(
        "Sample airport data created successfully"
    );
}

void Application::initializeSeatMaps()
{
    seatMaps.clear();

    for (const auto& flight :
         database.getFlights()) {

        const std::size_t capacity =
            flight->getPassengerCapacity();

        std::size_t rows = 0;
        std::size_t seatsPerRow = 6;

        if (capacity <= 180) {
            rows = 30;
        }
        else {
            rows = 40;
        }

        auto seatMap =
            std::make_shared<SeatMap>(
                rows,
                seatsPerRow,
                2,
                4,
                4
            );

        seatMaps[
            flight->getFlightNumber()
        ] = seatMap;
    }
}

void Application::synchronizeSeatMaps()
{
    for (const auto& booking :
         database.getBookings()) {

        if (!booking->hasSeat()) {
            continue;
        }

        const std::string flightNumber =
            booking
                ->getFlight()
                ->getFlightNumber();

        const auto iterator =
            seatMaps.find(
                flightNumber
            );

        if (iterator ==
            seatMaps.end()) {

            continue;
        }

        const std::string seatNumber =
            booking
                ->getSeat()
                ->getSeatNumber();

        if (!iterator
                 ->second
                 ->seatExists(seatNumber)) {

            continue;
        }

        if (iterator
                ->second
                ->isSeatAvailable(
                    seatNumber
                )) {

            iterator
                ->second
                ->reserveSeat(
                    seatNumber
                );
        }
    }
}

void Application::registerSeatMaps()
{
    for (const auto& pair :
         seatMaps) {

        menu.registerSeatMap(
            pair.first,
            pair.second
        );
    }
}

void Application::run()
{
    try {

        std::cout
            << "\n"
            << "========================================\n"
            << "     STARTING AIRPORT CHECK-IN SYSTEM\n"
            << "========================================\n";

        logger.info(
            "Application started"
        );

        if (hasSavedData()) {

            std::cout
                << "Loading saved data...\n";

            fileManager.loadAll(
                database
            );

            logger.info(
                "Saved data loaded successfully"
            );
        }
        else {

            std::cout
                << "No saved data found.\n"
                << "Creating sample data...\n";

            initializeSampleData();

            fileManager.saveAll(
                database
            );

            logger.info(
                "Initial data saved"
            );
        }

        initializeSeatMaps();

        synchronizeSeatMaps();

        registerSeatMaps();

        std::cout
            << "System initialized successfully.\n";

        database.printSummary();

        menu.run();

        fileManager.saveAll(
            database
        );

        logger.info(
            "Database saved successfully"
        );

        logger.info(
            "Application terminated normally"
        );
    }
    catch (const std::exception& exception) {

        std::cerr
            << "\n[FATAL ERROR] "
            << exception.what()
            << '\n';

        try {
            logger.error(
                std::string(
                    "Fatal application error: "
                ) +
                exception.what()
            );
        }
        catch (...) {
        }

        throw;
    }
}