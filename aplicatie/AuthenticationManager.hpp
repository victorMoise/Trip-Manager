#include "./AdminOrUser.hpp"
#include "./Rsa.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>


class AuthenticationManager {
private:
    RSA rsa;
    std::vector<AdminOrUser> users; // Assuming users data is loaded into this vector


    // Verifies the users credentials and if they exist in the datebase
    AdminOrUser* verifyCredentials(const std::string& username, const std::string& inputPassword) {
        std::string encryptedInputPassword = rsa.encrypt(inputPassword);

        for (int i = 0; i < users.size(); i++) {
            if (username == users[i].getUsername() && encryptedInputPassword == users[i].getPassword()) {
                return &users[i]; // Return a pointer to the User object
            }
        }

        return nullptr; // No matching credentials found
    }


    // Loads all available users from the datebase, stores them in the Object's vector
    void loadUserDataFromFile() {
        try {
            std::ifstream file("./storage/credentials.csv");

            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file!");
            }

            std::string line;

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string storedUsername, storedEncryptedPassword;

                if (std::getline(iss, storedUsername, ',') && std::getline(iss, storedEncryptedPassword)) {
                    users.emplace_back(storedUsername, storedEncryptedPassword);
                }
            }

            file.close();
        } catch (const std::exception& e) {
            std::cout << "Exception occurred: " << e.what() << std::endl;
        }
    }

public:
    // Constrcutor initializes the rsa class and loads the users from the .csv
    AuthenticationManager() : rsa(61, 53) {
        loadUserDataFromFile();
    }


    // Main function that promps the user to input a username and a password
    // until they are found in the database
    AdminOrUser loginUser() {
        std::string inputUsername;
        std::string inputPassword;

        while (true) {
            loadUserDataFromFile();
            std::cout << "\n-----------------------\n";
            std::cout << "Enter username (your email): ";
            std::cin >> inputUsername;
            std::cout << "Enter password: ";
            std::cin >> inputPassword;

            
            try {
                AdminOrUser* user = verifyCredentials(inputUsername, inputPassword);
                if (user != nullptr) {
                    return *user; // Return the User object
                } else {
                    throw std::runtime_error("Invalid username or password.");
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
};