#pragma once

#include <cstddef>
#include <string>

namespace Utils {

std::string trim(
    const std::string& text
);

std::string toUpper(
    const std::string& text
);

std::string toLower(
    const std::string& text
);

bool isNumber(
    const std::string& text
);

std::string readLine(
    const std::string& prompt
);

std::string readNonEmptyLine(
    const std::string& prompt
);

int readInt(
    const std::string& prompt,
    int minValue,
    int maxValue
);

double readDouble(
    const std::string& prompt,
    double minValue
);

std::string generateCode(
    const std::string& prefix,
    std::size_t randomLength
);

void printSeparator(
    char character = '=',
    std::size_t length = 50
);

void pressEnterToContinue();

}