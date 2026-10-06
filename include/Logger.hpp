#pragma once

#include <string>

enum class LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
private:
    std::string filePath;

    std::string getCurrentDateTime() const;
    std::string getLogLevelString(LogLevel level) const;

public:
    explicit Logger(const std::string& filePath);

    const std::string& getFilePath() const;

    void log(
        LogLevel level,
        const std::string& message
    ) const;

    void info(
        const std::string& message
    ) const;

    void warning(
        const std::string& message
    ) const;

    void error(
        const std::string& message
    ) const;

    void clear() const;

    void printLogFile() const;
};