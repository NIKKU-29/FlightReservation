#include "Flight.hpp"
#include <iostream>

Flight::Flight(const std::string& flightNumber, const std::string& airline,
               const std::string& origin, const std::string& destination,
               const std::string& departureTime, int capacity, int bookedSeats,
               int distance, int charge)
    : flightNumber(flightNumber), airline(airline), origin(origin),
      destination(destination), departureTime(departureTime), capacity(capacity),
      bookedSeats(bookedSeats), distance(distance), charge(charge) {}

void Flight::display() const {
    std::cout << "Flight Number: " << flightNumber << "\n"
              << "Airline: " << airline << "\n"
              << "Origin: " << origin << "\n"
              << "Destination: " << destination << "\n"
              << "Departure Time: " << departureTime << "\n"
              << "Capacity: " << capacity << "\n"
              << "Booked Seats: " << bookedSeats << "\n"
              << "Distance: " << distance << " km\n"
              << "Charge: " << charge << " INR\n";
}

double Flight::calculateOccupancyRate() const {
    return static_cast<double>(bookedSeats) / capacity * 100;
}
