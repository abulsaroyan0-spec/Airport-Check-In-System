#pragma once

#include "Flight.hpp"
#include "Luggage.hpp"
#include "Passenger.hpp"
#include "Seat.hpp"

#include <memory>
#include <string>
#include <vector>

enum class BookingStatus {
    Confirmed,
    CheckedIn,
    Cancelled,
    Completed
};

class Booking {
private:
    std::string bookingCode;

    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    std::shared_ptr<Seat> seat;

    std::vector<std::shared_ptr<Luggage>> luggageList;

    BookingStatus status;

public:
    Booking(
        const std::string& bookingCode,
        std::shared_ptr<Passenger> passenger,
        std::shared_ptr<Flight> flight
    );

    const std::string& getBookingCode() const;

    const std::shared_ptr<Passenger>& getPassenger() const;
    const std::shared_ptr<Flight>& getFlight() const;
    const std::shared_ptr<Seat>& getSeat() const;

    const std::vector<std::shared_ptr<Luggage>>&
    getLuggageList() const;

    BookingStatus getStatus() const;

    bool hasSeat() const;
    bool hasLuggage() const;

    std::size_t getLuggageCount() const;

    double getTotalLuggageWeight() const;
    double getTotalExtraLuggageFee() const;

    void assignSeat(
        std::shared_ptr<Seat> seat
    );

    void removeSeat();

    void addLuggage(
        std::shared_ptr<Luggage> luggage
    );

    void removeLuggage(
        const std::string& tagNumber
    );

    std::shared_ptr<Luggage> findLuggage(
        const std::string& tagNumber
    ) const;

    void checkIn();
    void cancel();
    void complete();

    std::string getStatusString() const;

    void print() const;
};