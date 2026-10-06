#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

Logger::Logger(
    const std::string& filePath
)
    : filePath(filePath)
{
    if (filePath.empty()) {
        throw std::invalid_argument(
            "Log file path cannot be empty"
        );
    }

    std::ofstream file(
        filePath,
        std::ios::app
    );

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot open log file: " + filePath
        );
    }
}

const std::string&
Logger::getFilePath() const
{
    return filePath;
}

std::string
Logger::getCurrentDateTime() const
{
    const auto now =
        std::chrono::system_clock::now();

    const std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    const std::tm* localTime =
        std::localtime(&currentTime);

    if (!localTime) {
        return "Unknown Time";
    }

    std::ostringstream stream;

    stream
        << std::put_time(
               localTime,
               "%Y-%m-%d %H:%M:%S"
           );

    return stream.str();
}

std::string
Logger::getLogLevelString(
    LogLevel level
) const
{
    switch (level) {

        case LogLevel::Info:
            return "INFO";

        case LogLevel::Warning:
            return "WARNING";

        case LogLevel::Error:
            return "ERROR";
    }

    return "UNKNOWN";
}

void Logger::log(
    LogLevel level,
    const std::string& message
) const
{
    if (message.empty()) {
        throw std::invalid_argument(
            "Log message cannot be empty"
        );
    }

    std::ofstream file(
        filePath,
        std::ios::app
    );

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot open log file: " + filePath
        );
    }

    file
        << '['
        << getCurrentDateTime()
        << "] ["
        << getLogLevelString(level)
        << "] "
        << message
        << '\n';
}

void Logger::info(
    const std::string& message
) const
{
    log(
        LogLevel::Info,
        message
    );
}

void Logger::warning(
    const std::string& message
) const
{
    log(
        LogLevel::Warning,
        message
    );
}

void Logger::error(
    const std::string& message
) const
{
    log(
        LogLevel::Error,
        message
    );
}

void Logger::clear() const
{
    std::ofstream file(
        filePath,
        std::ios::trunc
    );

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot clear log file: " + filePath
        );
    }
}

void Logger::printLogFile() const
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot open log file: " + filePath
        );
    }

    std::cout
        << "\n========================================\n"
        << "               SYSTEM LOG\n"
        << "========================================\n";

    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << '\n';
    }

    std::cout
        << "========================================\n";
}