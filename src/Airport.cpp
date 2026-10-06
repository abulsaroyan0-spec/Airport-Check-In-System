#include "Airport.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace {
bool isValidAirportCode(const std::string& code)
{
    if (code.size() != 3) {
        return false;
    }

    return std::all_of(
        code.begin(),
        code.end(),
        [](unsigned char character) {
            return std::isalpha(character);
        }
    );
}
}

Airport::Airport(
    const std::string& code,
    const std::string& name,
    const std::string& city,
    const std::string& country
)
    : code(code),
      name(name),
      city(city),
      country(country)
{
    if (!isValidAirportCode(code)) {
        throw std::invalid_argument(
            "Airport code must contain exactly 3 letters"
        );
    }

    if (name.empty()) {
        throw std::invalid_argument(
            "Airport name cannot be empty"
        );
    }

    if (city.empty()) {
        throw std::invalid_argument(
            "Airport city cannot be empty"
        );
    }

    if (country.empty()) {
        throw std::invalid_argument(
            "Airport country cannot be empty"
        );
    }

    std::transform(
        this->code.begin(),
        this->code.end(),
        this->code.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::toupper(character));
        }
    );
}

const std::string& Airport::getCode() const
{
    return code;
}

const std::string& Airport::getName() const
{
    return name;
}

const std::string& Airport::getCity() const
{
    return city;
}

const std::string& Airport::getCountry() const
{
    return country;
}

void Airport::setName(const std::string& name)
{
    if (name.empty()) {
        throw std::invalid_argument(
            "Airport name cannot be empty"
        );
    }

    this->name = name;
}

void Airport::setCity(const std::string& city)
{
    if (city.empty()) {
        throw std::invalid_argument(
            "Airport city cannot be empty"
        );
    }

    this->city = city;
}

void Airport::setCountry(const std::string& country)
{
    if (country.empty()) {
        throw std::invalid_argument(
            "Airport country cannot be empty"
        );
    }

    this->country = country;
}

std::string Airport::getFullLocation() const
{
    return city + ", " + country;
}

void Airport::print() const
{
    std::cout << "========================================\n";
    std::cout << "Airport Information\n";
    std::cout << "========================================\n";
    std::cout << "Code: " << code << '\n';
    std::cout << "Name: " << name << '\n';
    std::cout << "City: " << city << '\n';
    std::cout << "Country: " << country << '\n';
    std::cout << "========================================\n";
}