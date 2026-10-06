#pragma once

#include "Database.hpp"

#include <string>
#include <vector>

class FileManager {
private:
    std::string dataDirectory;

    std::string getAirportsFilePath() const;
    std::string getPassengersFilePath() const;
    std::string getFlightsFilePath() const;
    std::string getBookingsFilePath() const;
    std::string getLuggageFilePath() const;

    std::vector<std::string> split(
        const std::string& line,
        char delimiter
    ) const;

public:
    explicit FileManager(
        const std::string& dataDirectory
    );

    const std::string& getDataDirectory() const;

    void saveAll(
        const Database& database
    ) const;

    void loadAll(
        Database& database
    ) const;

    void clearDataFiles() const;
};