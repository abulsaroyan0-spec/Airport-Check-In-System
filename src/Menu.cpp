#include "Menu.hpp"

#include "Luggage.hpp"
#include "Payment.hpp"
#include "Utils.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

Menu::Menu(
    Database& database,
    Logger& logger
)
    : database(database),
      logger(logger)
{
}

void Menu::registerSeatMap(
    const std::string& flightNumber,
    std::shared_ptr<SeatMap> seatMap
)
{
    if (flightNumber.empty()) {
        throw std::invalid_argument(
            "Flight number cannot be empty"
        );
    }

    if (!seatMap) {
        throw std::invalid_argument(
            "Seat map cannot be null"
        );
    }

    database.findFlight(
        flightNumber
    );

    seatMaps[flightNumber] =
        std::move(seatMap);
}

void Menu::showMainMenu() const
{
    std::cout
        << "\n"
        << "====================================================\n"
        << "            AIRPORT CHECK-IN SYSTEM\n"
        << "====================================================\n"
        << "1. Flight Information\n"
        << "2. Find Booking\n"
        << "3. Passenger Check-In\n"
        << "4. Passenger List\n"
        << "5. Airport List\n"
        << "6. Database Summary\n"
        << "0. Exit\n"
        << "====================================================\n";
}

void Menu::handleListFlights() const
{
    const auto& flights =
        database.getFlights();

    std::cout
        << "\n"
        << "============================================================\n"
        << "                         FLIGHTS\n"
        << "============================================================\n";

    if (flights.empty()) {
        std::cout
            << "No flights available.\n"
            << "============================================================\n";

        return;
    }

    std::cout
        << std::left
        << std::setw(12)
        << "Flight"
        << std::setw(18)
        << "Route"
        << std::setw(14)
        << "Date"
        << std::setw(10)
        << "Time"
        << std::setw(10)
        << "Gate"
        << std::setw(15)
        << "Status"
        << '\n';

    std::cout
        << "------------------------------------------------------------\n";

    for (const auto& flight : flights) {

        std::cout
            << std::left
            << std::setw(12)
            << flight->getFlightNumber()

            << std::setw(18)
            << flight->getRoute()

            << std::setw(14)
            << flight->getDepartureDate()

            << std::setw(10)
            << flight->getDepartureTime()

            << std::setw(10)
            << flight->getGate()

            << std::setw(15)
            << flight->getStatusString()

            << '\n';
    }

    std::cout
        << "============================================================\n";
}

void Menu::handleFindBooking() const
{
    std::string bookingCode =
        Utils::readNonEmptyLine(
            "Enter booking code: "
        );

    bookingCode =
        Utils::toUpper(
            bookingCode
        );

    const auto booking =
        database.findBooking(
            bookingCode
        );

    booking->print();
}

void Menu::handleListPassengers() const
{
    const auto& passengers =
        database.getPassengers();

    if (passengers.empty()) {
        std::cout
            << "\nNo passengers found.\n";

        return;
    }

    std::cout
        << "\n========================================\n"
        << "             PASSENGER LIST\n"
        << "========================================\n";

    for (const auto& passenger :
         passengers) {

        passenger->print();
    }
}

void Menu::handleListAirports() const
{
    const auto& airports =
        database.getAirports();

    if (airports.empty()) {
        std::cout
            << "\nNo airports found.\n";

        return;
    }

    std::cout
        << "\n========================================\n"
        << "               AIRPORTS\n"
        << "========================================\n";

    for (const auto& airport :
         airports) {

        airport->print();
    }
}

void Menu::handleDatabaseSummary() const
{
    database.printSummary();
}

void Menu::addLuggageFlow(
    const std::shared_ptr<CheckIn>& checkIn
)
{
    while (true) {

        std::cout
            << "\n----------------------------------------\n"
            << "Add luggage?\n"
            << "1. Yes\n"
            << "2. No\n"
            << "----------------------------------------\n";

        const int choice =
            Utils::readInt(
                "Select: ",
                1,
                2
            );

        if (choice == 2) {
            break;
        }

        std::cout
            << "\n"
            << "Select luggage type:\n"
            << "1. Cabin\n"
            << "2. Checked\n"
            << "3. Oversized\n"
            << "4. Special\n";

        const int typeChoice =
            Utils::readInt(
                "Select type: ",
                1,
                4
            );

        LuggageType luggageType =
            LuggageType::Checked;

        double maxAllowedWeight =
            23.0;

        switch (typeChoice) {

            case 1:
                luggageType =
                    LuggageType::Cabin;

                maxAllowedWeight =
                    8.0;

                break;

            case 2:
                luggageType =
                    LuggageType::Checked;

                maxAllowedWeight =
                    23.0;

                break;

            case 3:
                luggageType =
                    LuggageType::Oversized;

                maxAllowedWeight =
                    32.0;

                break;

            case 4:
                luggageType =
                    LuggageType::Special;

                maxAllowedWeight =
                    23.0;

                break;
        }

        const double weight =
            Utils::readDouble(
                "Enter luggage weight (kg): ",
                0.0
            );

        const std::string tagNumber =
            Utils::generateCode(
                "BAG",
                7
            );

        constexpr double extraFeePerKg =
            15.0;

        auto luggage =
            std::make_shared<Luggage>(
                tagNumber,
                weight,
                maxAllowedWeight,
                extraFeePerKg,
                luggageType
            );

        checkIn->addLuggage(
            luggage
        );

        std::cout
            << "\nLuggage added successfully.\n";

        luggage->print();

        logger.info(
            "Luggage " +
            tagNumber +
            " added to booking " +
            checkIn
                ->getBooking()
                ->getBookingCode()
        );
    }
}

void Menu::processLuggagePayment(
    const std::shared_ptr<Booking>& booking,
    double amount
)
{
    if (!booking) {
        throw std::invalid_argument(
            "Booking cannot be null"
        );
    }

    if (amount <= 0.0) {
        return;
    }

    std::cout
        << "\n========================================\n"
        << "          EXTRA LUGGAGE PAYMENT\n"
        << "========================================\n"
        << std::fixed
        << std::setprecision(2)
        << "Amount Due: $"
        << amount
        << '\n'
        << "----------------------------------------\n"
        << "1. Cash\n"
        << "2. Card\n"
        << "3. Online\n"
        << "========================================\n";

    const int choice =
        Utils::readInt(
            "Payment method: ",
            1,
            3
        );

    PaymentMethod method =
        PaymentMethod::Cash;

    switch (choice) {

        case 1:
            method =
                PaymentMethod::Cash;
            break;

        case 2:
            method =
                PaymentMethod::Card;
            break;

        case 3:
            method =
                PaymentMethod::Online;
            break;
    }

    const std::string transactionId =
        Utils::generateCode(
            "TX",
            10
        );

    Payment payment(
        transactionId,
        booking,
        amount,
        "USD",
        method
    );

    payment.process();

    std::cout
        << "\nPayment completed successfully.\n";

    payment.print();

    logger.info(
        "Payment " +
        transactionId +
        " completed for booking " +
        booking->getBookingCode()
    );
}

void Menu::handlePassengerCheckIn()
{
    std::cout
        << "\n========================================\n"
        << "            PASSENGER CHECK-IN\n"
        << "========================================\n";

    std::string bookingCode =
        Utils::readNonEmptyLine(
            "Enter booking code: "
        );

    bookingCode =
        Utils::toUpper(
            bookingCode
        );

    auto booking =
        database.findBooking(
            bookingCode
        );

    if (booking->getStatus() ==
        BookingStatus::Cancelled) {

        throw std::runtime_error(
            "This booking is cancelled"
        );
    }

    if (booking->getStatus() ==
        BookingStatus::CheckedIn) {

        throw std::runtime_error(
            "Passenger is already checked in"
        );
    }

    if (booking->getStatus() ==
        BookingStatus::Completed) {

        throw std::runtime_error(
            "This booking is already completed"
        );
    }

    const auto flight =
        booking->getFlight();

    if (flight->getStatus() ==
        FlightStatus::Cancelled) {

        throw std::runtime_error(
            "Cannot check in because flight is cancelled"
        );
    }

    if (flight->getStatus() ==
        FlightStatus::Departed) {

        throw std::runtime_error(
            "Cannot check in because flight has departed"
        );
    }

    if (flight->getStatus() ==
        FlightStatus::Arrived) {

        throw std::runtime_error(
            "Cannot check in because flight has already arrived"
        );
    }

    const auto seatMapIterator =
        seatMaps.find(
            flight->getFlightNumber()
        );

    if (seatMapIterator ==
        seatMaps.end()) {

        throw std::runtime_error(
            "Seat map is not registered for flight " +
            flight->getFlightNumber()
        );
    }

    auto checkIn =
        std::make_shared<CheckIn>(
            booking,
            seatMapIterator->second
        );

    std::cout
        << "\nPassenger found:\n"
        << "Name: "
        << booking
               ->getPassenger()
               ->getFullName()
        << '\n'
        << "Flight: "
        << flight->getFlightNumber()
        << '\n'
        << "Route: "
        << flight->getRoute()
        << '\n'
        << "Departure: "
        << flight->getDepartureDate()
        << ' '
        << flight->getDepartureTime()
        << '\n';

    seatMapIterator
        ->second
        ->printSeatMap();

    std::string seatNumber =
        Utils::readNonEmptyLine(
            "Select seat: "
        );

    seatNumber =
        Utils::toUpper(
            seatNumber
        );

    checkIn->selectSeat(
        seatNumber
    );

    logger.info(
        "Seat " +
        seatNumber +
        " selected for booking " +
        bookingCode
    );

    std::cout
        << "\nSeat "
        << seatNumber
        << " selected successfully.\n";

    addLuggageFlow(
        checkIn
    );

    const double extraFee =
        checkIn->getExtraLuggageFee();

    std::cout
        << "\n----------------------------------------\n"
        << std::fixed
        << std::setprecision(2)
        << "Total luggage weight: "
        << booking->getTotalLuggageWeight()
        << " kg\n"
        << "Extra luggage fee: $"
        << extraFee
        << '\n'
        << "----------------------------------------\n";

    if (extraFee > 0.0) {

        processLuggagePayment(
            booking,
            extraFee
        );
    }

    const std::string boardingTime =
        Utils::readNonEmptyLine(
            "Enter boarding time (HH:MM): "
        );

    const int boardingGroup =
        Utils::readInt(
            "Enter boarding group (1-9): ",
            1,
            9
        );

    const std::string boardingPassNumber =
        Utils::generateCode(
            "BP",
            8
        );

    checkIn->completeCheckIn(
        boardingPassNumber,
        boardingTime,
        boardingGroup
    );

    logger.info(
        "Check-in completed for booking " +
        bookingCode
    );

    checkIn->printSummary();

    std::cout
        << "\nBoarding pass generated successfully.\n";

    const auto boardingPass =
        checkIn->getBoardingPass();

    if (boardingPass) {
        boardingPass->print();
    }
}

void Menu::run()
{
    logger.info(
        "Airport Check-In System menu started"
    );

    bool running = true;

    while (running) {

        try {

            showMainMenu();

            const int choice =
                Utils::readInt(
                    "Select option: ",
                    0,
                    6
                );

            switch (choice) {

                case 1:
                    handleListFlights();
                    break;

                case 2:
                    handleFindBooking();
                    break;

                case 3:
                    handlePassengerCheckIn();
                    break;

                case 4:
                    handleListPassengers();
                    break;

                case 5:
                    handleListAirports();
                    break;

                case 6:
                    handleDatabaseSummary();
                    break;

                case 0:
                    running = false;

                    logger.info(
                        "User exited Airport Check-In System"
                    );

                    std::cout
                        << "\nGoodbye!\n";

                    break;
            }

            if (running) {
                Utils::pressEnterToContinue();
            }
        }
        catch (const std::exception& exception) {

            std::cout
                << "\n[ERROR] "
                << exception.what()
                << '\n';

            try {
                logger.error(
                    exception.what()
                );
            }
            catch (...) {
                // Logging failure must not terminate the menu.
            }

            Utils::pressEnterToContinue();
        }
    }
}