#include <iostream>
#include <fstream>
#include <sstream>
#include "./Trip.hpp" // has the class Trip, contains data about each individual Trip


class TripManager {
private:
    std::vector<Trip> trips;
    
public:
    void loadTripsFromCSV(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open the file.");
            }

            std::string line;
            // Read and discard the header line if present
            std::getline(file, line);

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string departure, destination, handle, date;
                float price;

                if (std::getline(iss, departure, ',') &&
                    std::getline(iss, destination, ',') &&
                    std::getline(iss, handle, ',') &&
                    std::getline(iss, date, ',') &&
                    (iss >> price)) {
                    trips.emplace_back(departure, destination, date, price);
                } else {
                    throw std::runtime_error("Error parsing line: " + line);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << "\n";
        }
    }


    void printTrips() const {
        for (size_t i = 0; i < trips.size(); ++i) {
            Trip trip = trips[i];
            std::cout << trip.getAllInfo();
        }
    }


    std::vector<Trip> getTrips() {
        return trips;
    }
};