#include "Luggage.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

Luggage::Luggage(
    const std::string& tagNumber,
    double weight,
    double maxAllowedWeight,
    double extraFeePerKg,
    LuggageType type
)
    : tagNumber(tagNumber),
      weight(weight),
      maxAllowedWeight(maxAllowedWeight),
      extraFeePerKg(extraFeePerKg),
      type(type),
      checkedIn(false)
{
    if (tagNumber.empty()) {
        throw std::invalid_argument(
            "Luggage tag number cannot be empty"
        );
    }

    if (weight < 0.0) {
        throw std::invalid_argument(
            "Luggage weight cannot be negative"
        );
    }

    if (maxAllowedWeight <= 0.0) {
        throw std::invalid_argument(
            "Maximum allowed weight must be greater than zero"
        );
    }

    if (extraFeePerKg < 0.0) {
        throw std::invalid_argument(
            "Extra fee per kilogram cannot be negative"
        );
    }
}

const std::string& Luggage::getTagNumber() const
{
    return tagNumber;
}

double Luggage::getWeight() const
{
    return weight;
}

double Luggage::getMaxAllowedWeight() const
{
    return maxAllowedWeight;
}

double Luggage::getExtraFeePerKg() const
{
    return extraFeePerKg;
}

LuggageType Luggage::getType() const
{
    return type;
}

bool Luggage::isCheckedIn() const
{
    return checkedIn;
}

bool Luggage::isOverweight() const
{
    return weight > maxAllowedWeight;
}

double Luggage::getExtraWeight() const
{
    if (!isOverweight()) {
        return 0.0;
    }

    return weight - maxAllowedWeight;
}

double Luggage::calculateExtraFee() const
{
    return getExtraWeight() * extraFeePerKg;
}

std::string Luggage::getTypeString() const
{
    switch (type) {
        case LuggageType::Cabin:
            return "Cabin";

        case LuggageType::Checked:
            return "Checked";

        case LuggageType::Oversized:
            return "Oversized";

        case LuggageType::Special:
            return "Special";
    }

    return "Unknown";
}

void Luggage::setWeight(double weight)
{
    if (weight < 0.0) {
        throw std::invalid_argument(
            "Luggage weight cannot be negative"
        );
    }

    if (checkedIn) {
        throw std::runtime_error(
            "Cannot change luggage weight after check-in"
        );
    }

    this->weight = weight;
}

void Luggage::checkIn()
{
    if (checkedIn) {
        throw std::runtime_error(
            "Luggage is already checked in"
        );
    }

    checkedIn = true;
}

void Luggage::cancelCheckIn()
{
    if (!checkedIn) {
        throw std::runtime_error(
            "Luggage is not checked in"
        );
    }

    checkedIn = false;
}

void Luggage::print() const
{
    std::cout
        << "========================================\n"
        << "Luggage Information\n"
        << "========================================\n";

    std::cout
        << "Tag Number: "
        << tagNumber
        << '\n';

    std::cout
        << "Type: "
        << getTypeString()
        << '\n';

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Weight: "
        << weight
        << " kg\n";

    std::cout
        << "Allowed Weight: "
        << maxAllowedWeight
        << " kg\n";

    std::cout
        << "Status: "
        << (checkedIn
                ? "Checked In"
                : "Not Checked In")
        << '\n';

    if (isOverweight()) {
        std::cout
            << "Extra Weight: "
            << getExtraWeight()
            << " kg\n";

        std::cout
            << "Extra Fee: $"
            << calculateExtraFee()
            << '\n';
    } else {
        std::cout
            << "Extra Weight: 0.00 kg\n"
            << "Extra Fee: $0.00\n";
    }

    std::cout
        << "========================================\n";
}