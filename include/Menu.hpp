#pragma once

#include "CheckIn.hpp"
#include "Database.hpp"
#include "Logger.hpp"
#include "SeatMap.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class Menu {
private:
    Database& database;
    Logger& logger;

    std::unordered_map<
        std::string,
        std::shared_ptr<SeatMap>
    > seatMaps;

    void showMainMenu() const;

    void handleListFlights() const;
    void handleFindBooking() const;
    void handlePassengerCheckIn();
    void handleListPassengers() const;
    void handleListAirports() const;
    void handleDatabaseSummary() const;

    void addLuggageFlow(
        const std::shared_ptr<CheckIn>& checkIn
    );

    void processLuggagePayment(
        const std::shared_ptr<Booking>& booking,
        double amount
    );

public:
    Menu(
        Database& database,
        Logger& logger
    );

    void registerSeatMap(
        const std::string& flightNumber,
        std::shared_ptr<SeatMap> seatMap
    );

    void run();
};