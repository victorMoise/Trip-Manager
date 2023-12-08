#include "./AdminOrUser.hpp"
#include "./Trip.hpp"

class User : public AdminOrUser {  
private:
    std::vector<Trip> reservations;


    // Password must be at least 8 characters long
    bool isStrongPassword(const std::string& password) {
        return (password.length() >= 8);
    }


public:
    // Constructor
    User(std::string username, std::string password) : AdminOrUser(username, password) {}


    // Check if a username (string) has @gmail.com at the end
    bool isValidEmail(const std::string& email) {
        size_t atIndex = email.find('@');
        size_t dotIndex = email.find('.', atIndex);

        return (atIndex != std::string::npos &&
                dotIndex != std::string::npos &&
                dotIndex > atIndex);
    }


    // This is the function that takes care of adding/signing-up users 
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

            // Close the credentials file
            readFile.close();

            // Open the credentials file in append mode
            std::ofstream file("./storage/credentials.csv", std::ios::app); // Open file in append mode


            // Throw an error if the file was unable to be opened
            if (!file.is_open()) {
                throw std::runtime_error("Error: Unable to open file.\n");
            }


            // Throw an error if the email does not end in @gmail.com 
            if (!isValidEmail(username)) {
                throw std::runtime_error("Error: username must end in @gmail.com\n");
            }


            // Throw an error if the password is not at least 8 characters long
            if (!isStrongPassword(password)) {
                throw std::runtime_error("Error: Password must be at least 8 characters long.\n");
            }


            // Creating another user with the name admin is not allowed
            if (username == "admin") {
                throw std::runtime_error("Error: Cannot create user with name admin.\n");
            }

            // Check if the username already exists
            if (std::find(existingUsernames.begin(), existingUsernames.end(), getUsername()) != existingUsernames.end()) {
                throw std::runtime_error("Error: User already exists!\n");
            }


            // Add the new user in the .csv and displat the succed message
            file << getUsername() << "," << rsa.encrypt(getPassword()) << "\n"; // Include newline
            file.close();
            std::cout << "User succesfuly created.\n\n";

        } catch (const std::exception& e) {
            // Catch any of the above errors
            std::cout << e.what() << std::endl;
        }
    }



    void searchAndBookTrip(std::vector<Trip>& trips) {
        try {

            // Input needed to search for a trip
            std::string to, from, date;
            std::cout << "\n-----------------------\n";
            std::cout << "Enter trip details:\n";
            std::cout << "From: ";
            std::cin >> from;
            std::cout << "To: ";
            std::cin >> to;
            std::cout << "Date (dd.mm.yyyy): ";
            std::cin >> date;


            // Make sure to and from are in the correct format (capitalized)
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

            
            // Keeps track if the desired trip is found
            bool found = false;


            // Loop through all trips
            for (int i = 0; i < trips.size(); i++) {
                Trip trip = trips[i];

                // Check if any match the user's input, if yes display it's details
                if (trip.getDepartureLocation() == from && trip.getDestinationLocation() == to && trip.getDate() == date) {
                    found = true;
                    std::cout << "\n-----------------------\n";
                    std::cout << "Trip details:\n";
                    std::cout << "Trip Handle: " << trip.getHandle();
                    std::cout << ", Date: " << trip.getDate();
                    std::cout << std::fixed << std::setprecision(2) << ", Price: $" << trip.getPrice() << "\n";


                    // After a trip is found, the user has the possibilty to book the trip 


                    std::string bookTrip;
                    bool validInput = false;

                    while (!validInput) {
                        try {

                            // Display the prompt to check wheter or not he wants to book it
                            std::cout << "\n-----------------------\n";
                            std::cout << "Do you want to book " << trip.getHandle() << " on " << trip.getDate() <<  "? (Y/N)\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> bookTrip;
                            bookTrip = capitalizeString(bookTrip);


                            // Anything else of Y/N throws an error
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

                    break; // No need to continue, user already found/booked trip
                }
            }
            
            // In case no trips were found that matched the user's needs
            if (!found) {
                std::cout << "\n-----------------------\n";
                std::cout << "No trips found.\n";
            }

        } catch (const std::exception& e) {
            // Catches all exceptions
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
};