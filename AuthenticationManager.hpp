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

    AdminOrUser* verifyCredentials(const std::string& username, const std::string& inputPassword) {
        std::string encryptedInputPassword = rsa.encrypt(inputPassword);

        for (int i = 0; i < users.size(); i++) {
            if (username == users[i].getUsername() && encryptedInputPassword == users[i].getPassword()) {
                return &users[i]; // Return a pointer to the User object
            }
        }
        return nullptr; // No matching credentials found
    }

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
    AuthenticationManager() : rsa(61, 53) {
        // Load user data from file into the users vector or any other data structure
        loadUserDataFromFile();
    }

    AdminOrUser loginUser() {
        std::string inputUsername;
        std::string inputPassword;

        while (true) {
            std::cout << "\n-----------------------\n";
            std::cout << "Enter username: ";
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