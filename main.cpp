#include <iostream>
#include <limits>
#include "./TripManager.hpp"
#include "./AuthenticationManager.hpp"
#include "./User.hpp"
#include "./Admin.hpp"



int main() {
    TripManager tripManager;
    tripManager.loadTripsFromCSV("./storage/trips.csv");
    std::vector<Trip> trips = tripManager.getTrips();

    AuthenticationManager authManager;
    AdminOrUser loggedInUser;
    int choice = 0;

    while (choice != 3) {
        std::cout << "-----------------------\n";
        std::cout << "1. Log In\n";
        std::cout << "2. Sign Up\n";
        std::cout << "3. Quit\n";
        std::cout << "Enter your choice: ";
        
        try {
            // if the input is not a number 1-3
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Invalid input. Please enter a number 1-3");
            }


            // login section
            if (choice == 1) {
                loggedInUser = authManager.loginUser();
                std::cout << "Logged in succesfuly\n";


                // if the user is anyone except the admin
                if (loggedInUser.getUsername() != "admin") {
                    // "promote" the logged in user to normal User
                    User user(loggedInUser.getUsername(), loggedInUser.getPassword());


                    while (true) {
                        try {
                            std::cout << "\n-----------------------\n";
                            int userChoice;
                            std::cout << "1. Display all trips\n";
                            std::cout << "2. Search for and book a trip\n";
                            std::cout << "3. Exit\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> userChoice;


                            if (userChoice == 1) {
                                user.displayAllTrips(trips);
                            } if (userChoice == 2) {
                                user.searchAndBookTrip(trips);
                            }  else if (userChoice == 3) {
                                std::cout << "\n-----------------------\n";
                                std::cout << "Exiting program...\n";
                                return 0;
                            } else {
                                throw std::invalid_argument("Invalid choice. Please enter a number 1-2");
                            }
                        } catch (const std::exception& e) {
                            std::cout << "Error: " << e.what() << std::endl;
                            // Clear input buffer to prevent an infinite loop in case of invalid input
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                }


                // if the user is the admin
                if (loggedInUser.getUsername() == "admin") {
                    Admin admin(loggedInUser.getUsername(), loggedInUser.getPassword());


                    while (true) {
                        try {
                            std::cout << "\n-----------------------\n";
                            int adminChoice;
                            std::cout << "1. Display all trips\n";
                            std::cout << "2. Add a trip\n";
                            std::cout << "3. Delete a trip\n";
                            std::cout << "4. Exit\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> adminChoice;


                            if (adminChoice == 1) {
                                admin.displayAllTrips(trips);
                            } else if (adminChoice == 2) {
                                admin.addTrip(trips);
                            } else if (adminChoice == 3) {
                                admin.deleteTrip(trips);
                            } else if (adminChoice == 4) {
                                std::cout << "\n-----------------------\n";
                                std::cout << "Exiting program...\n";
                                return 0;
                            } else {
                                throw std::invalid_argument("Invalid choice. Please enter a number 1-3");
                            }
                        } catch(const std::exception& e) {
                            std::cout << "Error: " << e.what() << std::endl;
                            // Clear input buffer to prevent an infinite loop in case of invalid input
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                }

                return 0;
            } 
            
            
            // sign up section
            else if (choice == 2) {
                std::cout << "\n-----------------------\n";


                // prompt the user to input the username and password for his account
                std::string username, password;
                std::cout << "Create new account:\n";
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;



                // create the new user with the given credentials and store it into the csv
                // with the password encrypted
                User newUser(username, password);
                newUser.addUserToCSV();

                return 0;
            } 
            
            
            // exit section
            else if (choice == 3) {
                std::cout << "\n-----------------------\n";
                std::cout << "Exiting program...\n" << std::endl;
                return 0;
            } 
            
            
            // invalid input
            else {
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
