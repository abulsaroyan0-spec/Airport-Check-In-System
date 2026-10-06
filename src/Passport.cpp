#include "Passport.hpp"

#include <iostream>
#include <stdexcept>

Passport::Passport(
    const std::string& passportNumber,
    const std::string& nationality,
    const std::string& expiryDate
)
    : passportNumber(passportNumber),
      nationality(nationality),
      expiryDate(expiryDate)
{
    if (passportNumber.empty()) {
        throw std::invalid_argument("Passport number cannot be empty");
    }

    if (nationality.empty()) {
        throw std::invalid_argument("Nationality cannot be empty");
    }

    if (expiryDate.empty()) {
        throw std::invalid_argument("Expiry date cannot be empty");
    }
}

const std::string& Passport::getPassportNumber() const
{
    return passportNumber;
}

const std::string& Passport::getNationality() const
{
    return nationality;
}

const std::string& Passport::getExpiryDate() const
{
    return expiryDate;
}

void Passport::print() const
{
    std::cout << "Passport Number: " << passportNumber << '\n';
    std::cout << "Nationality: " << nationality << '\n';
    std::cout << "Expiry Date: " << expiryDate << '\n';
}