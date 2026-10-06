#pragma once

#include <string>

class Airport {
private:
    std::string code;
    std::string name;
    std::string city;
    std::string country;

public:
    Airport(
        const std::string& code,
        const std::string& name,
        const std::string& city,
        const std::string& country
    );

    const std::string& getCode() const;
    const std::string& getName() const;
    const std::string& getCity() const;
    const std::string& getCountry() const;

    void setName(const std::string& name);
    void setCity(const std::string& city);
    void setCountry(const std::string& country);

    std::string getFullLocation() const;

    void print() const;
};