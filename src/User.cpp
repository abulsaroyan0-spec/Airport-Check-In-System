#include "User.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace {

bool isValidUsername(
    const std::string& username
)
{
    if (username.size() < 3 ||
        username.size() > 30) {

        return false;
    }

    return std::all_of(
        username.begin(),
        username.end(),
        [](unsigned char character) {
            return std::isalnum(character) ||
                   character == '_' ||
                   character == '-' ||
                   character == '.';
        }
    );
}

bool isValidPassword(
    const std::string& password
)
{
    if (password.size() < 6) {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;

    for (unsigned char character : password) {

        if (std::isalpha(character)) {
            hasLetter = true;
        }

        if (std::isdigit(character)) {
            hasDigit = true;
        }
    }

    return hasLetter && hasDigit;
}

}

User::User(
    const std::string& userId,
    const std::string& username,
    const std::string& password
)
    : userId(userId),
      username(username),
      password(password),
      active(true)
{
    if (userId.empty()) {
        throw std::invalid_argument(
            "User ID cannot be empty"
        );
    }

    if (!isValidUsername(username)) {
        throw std::invalid_argument(
            "Username must contain 3-30 letters, digits, '.', '-' or '_'"
        );
    }

    if (!isValidPassword(password)) {
        throw std::invalid_argument(
            "Password must contain at least 6 characters, "
            "including a letter and a digit"
        );
    }
}

const std::string&
User::getUserId() const
{
    return userId;
}

const std::string&
User::getUsername() const
{
    return username;
}

bool User::isActive() const
{
    return active;
}

bool User::authenticate(
    const std::string& password
) const
{
    if (!active) {
        return false;
    }

    return this->password == password;
}

void User::changePassword(
    const std::string& currentPassword,
    const std::string& newPassword
)
{
    if (!active) {
        throw std::runtime_error(
            "Inactive user cannot change password"
        );
    }

    if (this->password != currentPassword) {
        throw std::runtime_error(
            "Current password is incorrect"
        );
    }

    if (!isValidPassword(newPassword)) {
        throw std::invalid_argument(
            "New password must contain at least 6 characters, "
            "including a letter and a digit"
        );
    }

    if (currentPassword == newPassword) {
        throw std::invalid_argument(
            "New password must be different from current password"
        );
    }

    password = newPassword;
}

void User::activate()
{
    if (active) {
        throw std::runtime_error(
            "User is already active"
        );
    }

    active = true;
}

void User::deactivate()
{
    if (!active) {
        throw std::runtime_error(
            "User is already inactive"
        );
    }

    active = false;
}

void User::print() const
{
    std::cout
        << "\n========================================\n"
        << "             USER INFORMATION\n"
        << "========================================\n"
        << "User ID:  "
        << userId
        << '\n'
        << "Username: "
        << username
        << '\n'
        << "Role:     "
        << getRole()
        << '\n'
        << "Status:   "
        << (active ? "Active" : "Inactive")
        << '\n'
        << "========================================\n";
}