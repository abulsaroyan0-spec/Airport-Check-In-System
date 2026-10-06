#pragma once

#include "Database.hpp"
#include "FileManager.hpp"
#include "Logger.hpp"
#include "Menu.hpp"
#include "SeatMap.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class Application {
private:
    Database database;
    Logger logger;
    FileManager fileManager;
    Menu menu;

    std::unordered_map<
        std::string,
        std::shared_ptr<SeatMap>
    > seatMaps;

    void initializeSampleData();

    void initializeSeatMaps();

    void synchronizeSeatMaps();

    void registerSeatMaps();

    bool hasSavedData() const;

public:
    Application();

    void run();
};