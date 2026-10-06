#pragma once

#include "Passport.hpp"

#include <string>

class Passenger {
private:
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string dateOfBirth;
    Passport passport;

public:
    Passenger(
        const std::string& id,
        const std::string& firstName,
        const std::string& lastName,
        const std::string& dateOfBirth,
        const Passport& passport
    );

    const std::string& getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const std::string& getDateOfBirth() const;

    const Passport& getPassport() const;

    std::string getFullName() const;

    void print() const;
};