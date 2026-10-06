#pragma once

#include <string>

class Passport {
private:
    std::string passportNumber;
    std::string nationality;
    std::string expiryDate;

public:
    Passport(
        const std::string& passportNumber,
        const std::string& nationality,
        const std::string& expiryDate
    );

    const std::string& getPassportNumber() const;
    const std::string& getNationality() const;
    const std::string& getExpiryDate() const;

    void print() const;
};