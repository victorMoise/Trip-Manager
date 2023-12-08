#ifndef ADMINORUSER_HPP
#define ADMINORUSER_HPP


#include <string>
#include <vector>
#include "./Rsa.hpp"


class AdminOrUser {
protected:
    RSA rsa;
    std::string username;
    std::string password;


    // function used by admin and user to search/add/remove trips even if 
    // the string given as input is not capitalized or has random 
    // lowercase/uppercase characters
    std::string capitalizeString(const std::string& input) {
        std::string result = input;

        // Capitalize the first letter
        if (!result.empty()) {
            result[0] = toupper(result[0]);
        }

        // Convert the rest of the characters to lowercase
        for (size_t i = 1; i < result.size(); ++i) {
            result[i] = tolower(result[i]);
        }

        return result;
    }


    std::string getCurrentDate() {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::tm* timeInfo = std::localtime(&currentTime);

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << timeInfo->tm_mday << "." // Day
            << std::setfill('0') << std::setw(2) << timeInfo->tm_mon + 1 << "." // Month (+1 as January is 0)
            << (timeInfo->tm_year + 1900); // Year (add 1900 as tm_year is years since 1900)

        return oss.str();
    }


    bool isDateNotInPast(const std::string& date) {
        std::tm timeInfo = {}; // Initialize with zeros to avoid potential issues

        std::istringstream dateStream(date);
        dateStream >> std::get_time(&timeInfo, "%d.%m.%Y");

        if (dateStream.fail()) {
            std::cout << "Invalid date format.\n";
            return false;
        }

        std::string dateNow = getCurrentDate();

        std::tm nowInfo = {}; // Initialize with zeros to avoid potential issues
        dateStream.clear(); // Clear stream flags
        dateStream.str(dateNow); // Use dateNow string as input for stream
        dateStream >> std::get_time(&nowInfo, "%d.%m.%Y");

        if (dateStream.fail()) {
            std::cout << "Invalid current date format.\n";
            return false;
        }

        std::time_t time = std::mktime(&timeInfo);
        std::time_t nowTime = std::mktime(&nowInfo);

        return time >= nowTime;
    }

public:
    AdminOrUser(std::string username, std::string password) : username(username), password(password), rsa(61, 53) {}
    AdminOrUser() : username(""), password(""), rsa(61, 53) {}

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }

    // Other User functionalities
    void displayAllTrips(std::vector<Trip> trips) {
        std::cout << "\n-----------------------\n";
        std::cout << "List of all available trips:\n";
        for (int i = 0; i < trips.size(); i++) {
            Trip trip = trips[i];
            std::cout << i + 1 << ". From-To: " << trip.getHandle() << ", Date: " << trip.getDate() << ", Price: " << trip.getPrice() << "\n";
        }
    }
};


#endif