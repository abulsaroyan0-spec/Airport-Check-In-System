#include "Utils.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

std::string Utils::trim(
    const std::string& text
)
{
    const auto first =
        std::find_if_not(
            text.begin(),
            text.end(),
            [](unsigned char character) {
                return std::isspace(character);
            }
        );

    if (first == text.end()) {
        return "";
    }

    const auto last =
        std::find_if_not(
            text.rbegin(),
            text.rend(),
            [](unsigned char character) {
                return std::isspace(character);
            }
        ).base();

    return std::string(
        first,
        last
    );
}

std::string Utils::toUpper(
    const std::string& text
)
{
    std::string result = text;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char character) {
            return static_cast<char>(
                std::toupper(character)
            );
        }
    );

    return result;
}

std::string Utils::toLower(
    const std::string& text
)
{
    std::string result = text;

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char character) {
            return static_cast<char>(
                std::tolower(character)
            );
        }
    );

    return result;
}

bool Utils::isNumber(
    const std::string& text
)
{
    if (text.empty()) {
        return false;
    }

    return std::all_of(
        text.begin(),
        text.end(),
        [](unsigned char character) {
            return std::isdigit(character);
        }
    );
}

std::string Utils::readLine(
    const std::string& prompt
)
{
    std::cout << prompt;

    std::string input;

    if (!std::getline(
            std::cin,
            input
        )) {

        throw std::runtime_error(
            "Failed to read input"
        );
    }

    return trim(input);
}

std::string Utils::readNonEmptyLine(
    const std::string& prompt
)
{
    while (true) {

        const std::string input =
            readLine(prompt);

        if (!input.empty()) {
            return input;
        }

        std::cout
            << "Input cannot be empty. "
            << "Please try again.\n";
    }
}

int Utils::readInt(
    const std::string& prompt,
    int minValue,
    int maxValue
)
{
    if (minValue > maxValue) {
        throw std::invalid_argument(
            "Minimum value cannot be greater than maximum value"
        );
    }

    while (true) {

        const std::string input =
            readLine(prompt);

        std::stringstream stream(input);

        int value = 0;
        char extraCharacter = '\0';

        if ((stream >> value) &&
            !(stream >> extraCharacter)) {

            if (value >= minValue &&
                value <= maxValue) {

                return value;
            }
        }

        std::cout
            << "Please enter a number between "
            << minValue
            << " and "
            << maxValue
            << ".\n";
    }
}

double Utils::readDouble(
    const std::string& prompt,
    double minValue
)
{
    while (true) {

        const std::string input =
            readLine(prompt);

        std::stringstream stream(input);

        double value = 0.0;
        char extraCharacter = '\0';

        if ((stream >> value) &&
            !(stream >> extraCharacter)) {

            if (value >= minValue) {
                return value;
            }
        }

        std::cout
            << "Please enter a number greater than or equal to "
            << minValue
            << ".\n";
    }
}

std::string Utils::generateCode(
    const std::string& prefix,
    std::size_t randomLength
)
{
    if (randomLength == 0) {
        throw std::invalid_argument(
            "Random code length must be greater than zero"
        );
    }

    const std::string characters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    std::random_device randomDevice;

    std::mt19937 generator(
        randomDevice()
    );

    std::uniform_int_distribution<std::size_t>
        distribution(
            0,
            characters.size() - 1
        );

    std::string result =
        toUpper(prefix);

    for (std::size_t i = 0;
         i < randomLength;
         ++i) {

        result += characters[
            distribution(generator)
        ];
    }

    return result;
}

void Utils::printSeparator(
    char character,
    std::size_t length
)
{
    for (std::size_t i = 0;
         i < length;
         ++i) {

        std::cout << character;
    }

    std::cout << '\n';
}

void Utils::pressEnterToContinue()
{
    std::cout
        << "\nPress ENTER to continue...";

    std::string input;
    std::getline(
        std::cin,
        input
    );
}