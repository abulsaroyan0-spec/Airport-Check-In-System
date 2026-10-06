cmake_minimum_required(VERSION 3.16)

project(
    AirportCheckInSystem
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)

file(
    MAKE_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/data
)

add_executable(
    AirportCheckInSystem

    src/main.cpp

    src/Airport.cpp
    src/Application.cpp
    src/BoardingPass.cpp
    src/Booking.cpp
    src/CheckIn.cpp
    src/CheckInAgent.cpp
    src/Database.cpp
    src/Employee.cpp
    src/FileManager.cpp
    src/Flight.cpp
    src/Logger.cpp
    src/Luggage.cpp
    src/Menu.cpp
    src/Passenger.cpp
    src/Passport.cpp
    src/Payment.cpp
    src/Seat.cpp
    src/SeatMap.cpp
    src/User.cpp
    src/Utils.cpp
)

target_include_directories(
    AirportCheckInSystem
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(
        AirportCheckInSystem
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        AirportCheckInSystem
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()