#include "./AdminOrUser.hpp"
#include "./Trip.hpp"

class User : public AdminOrUser {  
private:
    std::vector<Trip> reservations;


public:
    User(std::string username, std::string password) : AdminOrUser(username, password) {}


    void addUserToCSV() {
        try {
            std::ifstream readFile("./storage/credentials.csv"); // Open file to read existing usernames

            std::string line;
            std::vector<std::string> existingUsernames;

            // Read existing usernames from the file
            while (std::getline(readFile, line)) {
                std::istringstream iss(line);
                std::string username;
                std::getline(iss, username, ',');
                existingUsernames.push_back(username);
            }

            readFile.close();

            std::ofstream file("./storage/credentials.csv", std::ios::app); // Open file in append mode

            if (!file.is_open()) {
                throw std::runtime_error("Error: Unable to open file.");
            }

            // Creating another user with the name admin is not allowed
            if (username == "admin") {
                throw std::runtime_error("Error: Cannot create user with name admin.");
            }

            // Check if the username already exists
            if (std::find(existingUsernames.begin(), existingUsernames.end(), getUsername()) != existingUsernames.end()) {
                throw std::runtime_error("Error: User already exists!");
            }


            file << getUsername() << "," << rsa.encrypt(getPassword()) << "\n"; // Include newline
            file.close();
            std::cout << "User succesfuly created.\n";
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }



    void searchAndBookTrip(std::vector<Trip>& trips) {
        try {
            std::string to, from, date;
            std::cout << "\n-----------------------\n";
            std::cout << "Enter trip details:\n";
            std::cout << "From: ";
            std::cin >> from;
            std::cout << "To: ";
            std::cin >> to;
            std::cout << "Date (dd.mm.yyyy): ";
            std::cin >> date;


            // make sure to and from are in the correct format
            to = capitalizeString(to);
            from = capitalizeString(from);


            // Date format checking
            if (date.size() != 10 || date[2] != '.' || date[5] != '.') {
                throw std::invalid_argument("Invalid date format. Please enter in dd.mm.yyyy format.");
            }

            // make sure the date is not in the past
            if (!isDateNotInPast(date)) {
                throw std::runtime_error("Date is in the past.");
            }


            bool found = false;

            for (int i = 0; i < trips.size(); i++) {
                Trip trip = trips[i];
                if (trip.getDepartureLocation() == from && trip.getDestinationLocation() == to && trip.getDate() == date) {
                    found = true;
                    std::cout << "\n-----------------------\n";
                    std::cout << "Trip details:\n";
                    std::cout << "Trip Handle: " << trip.getHandle();
                    std::cout << ", Date: " << trip.getDate();
                    std::cout << std::fixed << std::setprecision(2) << ", Price: $" << trip.getPrice() << "\n";


                    // if a trip is found, give the user the posibility to book it
                    std::string bookTrip;
                    bool validInput = false;

                    while (!validInput) {
                        try {
                            std::cout << "\n-----------------------\n";
                            std::cout << "Do you want to book " << trip.getHandle() << " on " << trip.getDate() <<  "? (Y/N)\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> bookTrip;
                            bookTrip = capitalizeString(bookTrip);

                            if (bookTrip != "Y" && bookTrip != "N") {
                                throw std::invalid_argument("Invalid input. Please enter Y or N.");
                            }
                            
                            // if the input is Y, then the user books the found trip
                            if (bookTrip == "Y") {
                                reservations.push_back(trip);
                                std::cout << "\n-----------------------\n";
                                std::cout << "Trip booked succesfuly\n";
                            }

                            validInput = true; // Set to true if input is valid
                        } catch (const std::exception& e) {
                            std::cout << "Error: " << e.what() << std::endl;
                            // Clear input buffer to prevent an infinite loop in case of invalid input
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }

                    break;
                }
            }

            if (!found) {
                std::cout << "\n-----------------------\n";
                std::cout << "No trips found.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }


    
};