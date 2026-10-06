#include "Application.hpp"

#include <exception>
#include <iostream>

int main()
{
    try {
        Application application;

        application.run();
    }
    catch (const std::exception& exception) {
        std::cerr
            << "\nApplication terminated with error:\n"
            << exception.what()
            << '\n';

        return 1;
    }
    catch (...) {
        std::cerr
            << "\nApplication terminated with an unknown error.\n";

        return 1;
    }

    return 0;
}