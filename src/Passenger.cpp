#include "Passenger.hpp"

#include <iostream>
#include <stdexcept>

Passenger::Passenger(
    const std::string& id,
    const std::string& firstName,
    const std::string& lastName,
    const std::string& dateOfBirth,
    const Passport& passport
)
    : id(id),
      firstName(firstName),
      lastName(lastName),
      dateOfBirth(dateOfBirth),
      passport(passport)
{
    if (id.empty()) {
        throw std::invalid_argument("Passenger ID cannot be empty");
    }

    if (firstName.empty()) {
        throw std::invalid_argument("First name cannot be empty");
    }

    if (lastName.empty()) {
        throw std::invalid_argument("Last name cannot be empty");
    }

    if (dateOfBirth.empty()) {
        throw std::invalid_argument("Date of birth cannot be empty");
    }
}

const std::string& Passenger::getId() const
{
    return id;
}

const std::string& Passenger::getFirstName() const
{
    return firstName;
}

const std::string& Passenger::getLastName() const
{
    return lastName;
}

const std::string& Passenger::getDateOfBirth() const
{
    return dateOfBirth;
}

const Passport& Passenger::getPassport() const
{
    return passport;
}

std::string Passenger::getFullName() const
{
    return firstName + " " + lastName;
}

void Passenger::print() const
{
    std::cout << "========================================\n";
    std::cout << "Passenger Information\n";
    std::cout << "========================================\n";

    std::cout << "ID: " << id << '\n';
    std::cout << "Name: " << getFullName() << '\n';
    std::cout << "Date of Birth: " << dateOfBirth << '\n';

    std::cout << "----------------------------------------\n";

    passport.print();

    std::cout << "========================================\n";
}