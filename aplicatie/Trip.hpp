#ifndef TRIP_HPP
#define TRIP_HPP


#include <string>

class Trip {
private:
    std::string departure;
    std::string destination;
    std::string handle; // for easier listing (looks like: departure-destination)
    std::string date;
    float price;
public: 
    // constructor, handle is created automatically based on departure and destination
    std::string createHandle() {
        return this->departure + "-" + this->destination;
    }

    Trip(std::string departure, std::string destination, std::string date, float price) {
        this->departure = departure;
        this->destination = destination;
        this->date = date;
        this->price = price;

        this->handle = createHandle();
    }

    std::string getAllInfo() {
        std::string info = "From-To: " + handle + ", Date: " + date + ", Price: " + std::to_string(price) + "\n"; 
        return info;
    }

    std::string getDepartureLocation() const {
        return this->departure;
    }

    std::string getDestinationLocation() const {
        return this->destination;
    }

    std::string getDate() const {
        return this->date;
    }

    std::string getHandle() const {
        return this->handle;
    }

    float getPrice() const {
        return this->price;
    }
};


#endif