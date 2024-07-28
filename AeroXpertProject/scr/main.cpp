#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include "C:/Users/Sanjay/Desktop/AeroXpertProject/include/json.hpp"

using json = nlohmann::json;
using namespace std;

class Flight {
public:
    string flightNumber;
    string airline;
    string origin;
    string destination;
    string departureTime;
    int capacity;
    int bookedSeats;
    int distance;
    double charge;

    Flight(string fn, string air, string orig, string dest, string deptTime, int cap, int dist, double chg)
        : flightNumber(fn), airline(air), origin(orig), destination(dest), departureTime(deptTime), 
          capacity(cap), bookedSeats(0), distance(dist), charge(chg) {}

    void display() const {
        cout << left << setw(15) << flightNumber
             << setw(15) << airline
             << setw(10) << origin
             << setw(13) << destination
             << setw(20) << departureTime
             << setw(10) << capacity
             << setw(13) << bookedSeats
             << setw(10) << distance
             << setw(10) << charge << endl;
    }
};

class BookingSystem {
private:
    vector<Flight> flights;
    map<string, vector<pair<string, string>>> adjacencyList;
    string employeePassword = "123456";

public:
    BookingSystem() {
        loadFlights();
    }

    void addFlight(const Flight& flight) {
        flights.push_back(flight);
        adjacencyList[flight.origin].push_back({flight.destination, flight.flightNumber});
        adjacencyList[flight.destination].push_back({flight.origin, flight.flightNumber});
        saveFlights();
    }

    void removeFlight(const string& flightNumber) {
        auto it = remove_if(flights.begin(), flights.end(), [&](const Flight& flight) {
            return flight.flightNumber == flightNumber;
        });
        if (it != flights.end()) {
            flights.erase(it, flights.end());
            saveFlights();
            cout << "Flight " << flightNumber << " removed successfully." << endl;
        } else {
            cout << "Flight " << flightNumber << " not found." << endl;
        }
    }

    void displayFlights() const {
        cout << left << setw(15) << "Flight No"
             << setw(15) << "Airline"
             << setw(10) << "Origin"
             << setw(13) << "Destination"
             << setw(20) << "Departure Time"
             << setw(10) << "Capacity"
             << setw(13) << "Booked Seats"
             << setw(10) << "Distance"
             << setw(10) << "Charge" << endl;
        cout << string(128, '-') << endl;

        for (const auto& flight : flights) {
            flight.display();
        }
        cout << string(128, '-') << endl;
    }

    vector<string> findConnectingFlights(const string& start, const string& destination) {
        vector<string> path;
        vector<string> queue;
        map<string, bool> visited;

        queue.push_back(start);

        while (!queue.empty()) {
            string current = queue.front();
            queue.erase(queue.begin());
            visited[current] = true;

            if (current == destination) {
                return path;
            }

            for (const auto& [neighbor, flightNumber] : adjacencyList[current]) {
                if (!visited[neighbor]) {
                    queue.push_back(neighbor);
                    path.push_back(flightNumber);
                }
            }
        }

        return path;
    }

    bool isFlightAvailable(const string& start, const string& destination) const {
        for (const auto& flight : flights) {
            if (flight.origin == start && flight.destination == destination) {
                return true;
            }
        }
        return false;
    }

    int getMinDistance(const string& start, const string& destination) {
        if (start == destination) {
            return 0;
        }

        vector<string> path = findConnectingFlights(start, destination);
        if (!path.empty()) {
            int total_distance = 0;
            for (const auto& flightNumber : path) {
                Flight flight = getFlightByNumber(flightNumber);
                total_distance += flight.distance;
            }
            return total_distance;
        }

        return -1;
    }

    Flight getFlightByNumber(const string& flightNumber) const {
        for (const auto& flight : flights) {
            if (flight.flightNumber == flightNumber) {
                return flight;
            }
        }
        throw runtime_error("Flight not found.");
    }

    void bookSeatsRandomly(const string& flightNumber, int num_seats) {
        for (auto& flight : flights) {
            if (flight.flightNumber == flightNumber) {
                for (int i = 0; i < num_seats; ++i) {
                    if (flight.bookedSeats < flight.capacity) {
                        flight.bookedSeats++;
                    } else {
                        cout << "No more seats available for flight " << flightNumber << "." << endl;
                        return;
                    }
                }
                saveFlights();
                return;
            }
        }
        cout << "Flight not found." << endl;
    }

    void displayCustomerBookings() const {
        cout << "Displaying Customer Bookings:" << endl;
        for (const auto& flight : flights) {
            if (flight.bookedSeats > 0) {
                cout << "Flight Number: " << flight.flightNumber << ", Booked Seats: " 
                     << flight.bookedSeats << "/" << flight.capacity << endl;
            }
        }
    }

    void bookSeats(const string& flightNumber, const vector<int>& seat_numbers) {
        try {
            Flight flight = getFlightByNumber(flightNumber);
            for (int seat : seat_numbers) {
                if (flight.bookedSeats < flight.capacity) {
                    flight.bookedSeats++;
                    cout << "Seat " << seat << " booked successfully for flight " << flightNumber << "." << endl;
                } else {
                    cout << "No more seats available for flight " << flightNumber << "." << endl;
                    return;
                }
            }
            saveFlights();
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
        }
    }

    bool employeeLogin(const string& password) const {
        return password == employeePassword;
    }

    vector<Flight> getAvailableFlights(const string& start, const string& destination) const {
        vector<Flight> availableFlights;
        for (const auto& flight : flights) {
            if (flight.origin == start && flight.destination == destination && flight.capacity - flight.bookedSeats > 0) {
                availableFlights.push_back(flight);
            }
        }
        return availableFlights;
    }

    void saveFlights() const {
        json flightData;
        for (const auto& flight : flights) {
            flightData.push_back({
                {"flightNumber", flight.flightNumber},
                {"airline", flight.airline},
                {"origin", flight.origin},
                {"destination", flight.destination},
                {"departureTime", flight.departureTime},
                {"capacity", flight.capacity},
                {"bookedSeats", flight.bookedSeats},
                {"distance", flight.distance},
                {"charge", flight.charge}
            });
        }

        ofstream file("flights.json");
        if (file.is_open()) {
            file << setw(4) << flightData << endl;
            file.close();
        }
    }

    void loadFlights() {
        ifstream file("flights.json");
        if (file.is_open()) {
            json flightData;
            file >> flightData;

            for (const auto& item : flightData) {
                Flight flight(
                    item["flightNumber"].get<string>(),
                    item["airline"].get<string>(),
                    item["origin"].get<string>(),
                    item["destination"].get<string>(),
                    item["departureTime"].get<string>(),
                    item["capacity"].get<int>(),
                    item["distance"].get<int>(),
                    item["charge"].get<double>()
                );
                flight.bookedSeats = item["bookedSeats"].get<int>();
                flights.push_back(flight);
                adjacencyList[flight.origin].push_back({flight.destination, flight.flightNumber});
                adjacencyList[flight.destination].push_back({flight.origin, flight.flightNumber});
            }

            file.close();
        }
    }

    static void clear_screen() {
        system("cls");
    }
};

int main() {
    BookingSystem bookingSystem;

    cout << "Welcome to the Flight Booking System!" << endl;

    while (true) {
        cout << "\n+" << string(38, '-') << "+" << endl;
        cout << "|" << setw(38) << "Menu" << "|" << endl;
        cout << "+" << string(38, '-') << "+" << endl;
        cout << "| 1. Employee Login                     |" << endl;
        cout << "| 2. Customer Booking                   |" << endl;
        cout << "| 3. Display Customer Bookings          |" << endl;
        cout << "| 4. Exit                               |" << endl;
        cout << "+" << string(38, '-') << "+" << endl;
        cout << "Enter your choice (1-4): ";
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue;
        }

        if (choice == 1) {
            string password;
            cout << "Enter employee password: ";
            cin >> password;
            if (bookingSystem.employeeLogin(password)) {
                cout << "Login successful!" << endl;
                int employeeChoice;
                while (true) {
                    cout << "\nEmployee Menu:" << endl;
                    cout << "1. Add Flight" << endl;
                    cout << "2. Remove Flight" << endl;
                    cout << "3. Display Flights" << endl;
                    cout << "4. Back to Main Menu" << endl;
                    cout << "Enter your choice (1-4): ";
                    cin >> employeeChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number between 1 and 4." << endl;
                        continue;
                    }

                    if (employeeChoice == 1) {
                        string flightNumber, airline, origin, destination, departureTime;
                        int capacity, distance;
                        double charge;

                        cout << "Enter flight number: ";
                        cin >> flightNumber;
                        cout << "Enter airline: ";
                        cin >> airline;
                        cout << "Enter origin: ";
                        cin >> origin;
                        cout << "Enter destination: ";
                        cin >> destination;
                        cout << "Enter departure time (HH:MM): ";
                        cin >> departureTime;
                        cout << "Enter capacity: ";
                        cin >> capacity;
                        cout << "Enter distance: ";
                        cin >> distance;
                        cout << "Enter charge: ";
                        cin >> charge;

                        Flight newFlight(flightNumber, airline, origin, destination, departureTime, capacity, distance, charge);
                        bookingSystem.addFlight(newFlight);
                        cout << "Flight added successfully!" << endl;
                    } else if (employeeChoice == 2) {
                        string flightNumber;
                        cout << "Enter flight number to remove: ";
                        cin >> flightNumber;
                        bookingSystem.removeFlight(flightNumber);
                    } else if (employeeChoice == 3) {
                        bookingSystem.displayFlights();
                    } else if (employeeChoice == 4) {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
                }
            } else {
                cout << "Invalid password. Please try again." << endl;
            }
        } else if (choice == 2) {
            string origin, destination;
            cout << "Enter origin: ";
            cin >> origin;
            cout << "Enter destination: ";
            cin >> destination;
            vector<Flight> availableFlights = bookingSystem.getAvailableFlights(origin, destination);
            if (!availableFlights.empty()) {
                cout << "Available Flights:" << endl;
                for (const auto& flight : availableFlights) {
                    flight.display();
                }

                string flightNumber;
                int num_seats;
                cout << "Enter flight number to book: ";
                cin >> flightNumber;
                cout << "Enter number of seats to book: ";
                cin >> num_seats;
                bookingSystem.bookSeatsRandomly(flightNumber, num_seats);
            } else {
                cout << "No flights available for the selected route." << endl;
            }
        } else if (choice == 3) {
            bookingSystem.displayCustomerBookings();
        } else if (choice == 4) {
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
