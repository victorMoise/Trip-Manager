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


    // 3 main choices, 1=log-in, 2=sign-up, 3=quit
    while (choice != 3) {
        std::cout << "-----------------------\n";
        std::cout << "1. Log In\n";
        std::cout << "2. Sign Up\n";
        std::cout << "3. Quit\n";
        std::cout << "Enter your choice: ";
        
        try {
            // Throw an error if the input is a anything else except 1,2,3
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Invalid input. Please enter a number 1-3");
            }


            // If the selected option is log-in
            if (choice == 1) {
                
                // Calls the function to login the user and display the message if succesful
                loggedInUser = authManager.loginUser();
                std::cout << "Logged in succesfuly\n";


                // If the user if anyone except the admin
                // there can only be ONE admin, and noone is allowed to have admin as their username
                if (loggedInUser.getUsername() != "admin") {
                    // "promote" the logged in user to normal User
                    User user(loggedInUser.getUsername(), loggedInUser.getPassword());


                    while (true) {
                        try {

                            // Display all of the user's options
                            std::cout << "\n-----------------------\n";
                            int userChoice;
                            std::cout << "1. Display all trips\n";
                            std::cout << "2. Search for and book a trip\n";
                            std::cout << "3. Exit\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> userChoice;


                            if (userChoice == 1) { // Displays all available trips
                                user.displayAllTrips(trips);
                            } else if (userChoice == 2) { // Searches and if need be book a trip
                                user.searchAndBookTrip(trips);
                            }  else if (userChoice == 3) { // Exit
                                std::cout << "\n-----------------------\n";
                                std::cout << "Exiting program...\n";
                                return 0;
                            } else {
                                // Throw an error if the inputted option is not a valid one
                                throw std::invalid_argument("Invalid choice. Please enter a number 1-3");
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
                    // "promote" the loggedInUser to be Admin
                    Admin admin(loggedInUser.getUsername(), loggedInUser.getPassword());


                    while (true) {
                        try {

                            // Display all of the admins options
                            std::cout << "\n-----------------------\n";
                            int adminChoice;
                            std::cout << "1. Display all trips\n";
                            std::cout << "2. Add a trip\n";
                            std::cout << "3. Delete a trip\n";
                            std::cout << "4. Exit\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> adminChoice;


                            if (adminChoice == 1) { // Also has the ability to view all available trip
                                admin.displayAllTrips(trips);
                            } else if (adminChoice == 2) { // He can also add a trips 
                                admin.addTrip(trips);
                            } else if (adminChoice == 3) { // Or delete one
                                admin.deleteTrip(trips);
                            } else if (adminChoice == 4) { // Exit
                                std::cout << "\n-----------------------\n";
                                std::cout << "Exiting program...\n";
                                return 0;
                            } else {
                                // Throw an error if the inputted option is not a valid one
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
                std::cout << "Email: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;



                // create the new user with the given credentials and store it into the csv
                // with the password encrypted
                User newUser(username, password);
                newUser.addUserToCSV();

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
