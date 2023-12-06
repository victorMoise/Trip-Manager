#include "./AdminOrUser.hpp"
#include "./Trip.hpp"

class Admin : public AdminOrUser {
public:
    Admin(std::string username, std::string password) : AdminOrUser(username, password) {}

    void addTrip(std::vector<Trip>& trips) {
        try {
            std::string to, from, date, handle;
            float price;

            std::cout << "\n-----------------------\n";
            std::cout << "Enter trip details to add:\n";
            std::cout << "From: ";
            std::cin >> from;
            std::cout << "To: ";
            std::cin >> to;
            std::cout << "Date (dd.mm.yyyy): ";
            std::cin >> date;
            std::cout << "Price: ";
            std::cin >> price;

            to = capitalizeString(to);
            from = capitalizeString(from);

            Trip trip(from, to, date, price);

            if (date.size() != 10 || date[2] != '.' || date[5] != '.') {
                throw std::invalid_argument("Invalid date format. Please enter in dd.mm.yyyy format.");
            }

            if (!isDateNotInPast(date)) {
                throw std::runtime_error("Date is in the past.");
            }

            // Check if the trip already exists in the vector
            for (int i = 0; i < trips.size(); i++) {
                Trip trip = trips[i];
                if (trip.getDepartureLocation() == from && trip.getDestinationLocation() == to && trip.getDate() == date) {
                    throw std::runtime_error("Trip already exists.");
                }
            }

            // Open file in append mode
            std::ofstream file("./storage/trips.csv", std::ios::app);

            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file.");
            }

            // Append trip details to the file
            file << from << "," << to << "," << trip.getHandle() << "," << date << "," << price << "\n";
            trips.push_back(trip);
            file.close();

            std::cout << "Trip added successfully.\n";

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            // Clear input buffer to prevent an infinite loop in case of invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }



    void deleteTrip(std::vector<Trip>& trips) {
        try {
            std::string to, from, date;
            float price;

            std::cout << "\n-----------------------\n";
            std::cout << "Enter trip details to remove:\n";
            std::cout << "From: ";
            std::cin >> from;
            std::cout << "To: ";
            std::cin >> to;
            std::cout << "Date (dd.mm.yyyy): ";
            std::cin >> date;



            to = capitalizeString(to);
            from = capitalizeString(from);

            Trip trip(from, to, date, price);

            if (date.size() != 10 || date[2] != '.' || date[5] != '.') {
                throw std::invalid_argument("Invalid date format. Please enter in dd.mm.yyyy format.");
            }

            // Find the trip in the vector
            auto it = trips.end(); // Initialize iterator to the end initially

            for (auto iter = trips.begin(); iter != trips.end(); ++iter) {
                if (iter->getDepartureLocation() == from && iter->getDestinationLocation() == to &&
                    iter->getDate() == date) {
                    it = iter; // Set the iterator to the found position
                    break;
                }
            }


            if (it == trips.end()) {
                throw std::runtime_error("Trip not found.");
            }


            // Erase trip from the vector
            trips.erase(it);

            // Write updated vector to the CSV file
            std::ofstream file("./storage/trips.csv");
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file.");
            }


            // print the top row again with the 
            file << "Departure,Destination,Handle,Date,Price\n";


            for (int i = 0; i < trips.size(); i++) {
                Trip currentTrip = trips[i];
                file << currentTrip.getDepartureLocation() << "," << currentTrip.getDestinationLocation() << "," << currentTrip.getHandle() << "," 
                << currentTrip.getDate() << "," << currentTrip.getPrice() << "\n";
            }
            file.close();

            std::cout << "Trip removed successfully.\n";

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            // Clear input buffer to prevent an infinite loop in case of invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
};