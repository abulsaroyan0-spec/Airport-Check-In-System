#pragma once

#include <string>

enum class LuggageType {
    Cabin,
    Checked,
    Oversized,
    Special
};

class Luggage {
private:
    std::string tagNumber;
    double weight;
    double maxAllowedWeight;
    double extraFeePerKg;

    LuggageType type;

    bool checkedIn;

public:
    Luggage(
        const std::string& tagNumber,
        double weight,
        double maxAllowedWeight,
        double extraFeePerKg,
        LuggageType type
    );

    const std::string& getTagNumber() const;

    double getWeight() const;
    double getMaxAllowedWeight() const;
    double getExtraFeePerKg() const;

    LuggageType getType() const;

    bool isCheckedIn() const;
    bool isOverweight() const;

    double getExtraWeight() const;
    double calculateExtraFee() const;

    std::string getTypeString() const;

    void setWeight(double weight);

    void checkIn();
    void cancelCheckIn();

    void print() const;
};