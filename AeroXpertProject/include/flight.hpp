#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <string>

class Flight {
public:
    std::string flightNumber;
    std::string airline;
    std::string origin;
    std::string destination;
    std::string departureTime;
    int capacity;
    int bookedSeats;
    int distance;
    int charge;

    Flight() = default;
    Flight(const std::string& flightNumber, const std::string& airline,
           const std::string& origin, const std::string& destination,
           const std::string& departureTime, int capacity, int bookedSeats,
           int distance, int charge);

    void display() const;
    double calculateOccupancyRate() const;
};

#endif // FLIGHT_HPP
