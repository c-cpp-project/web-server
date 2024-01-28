#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <stdexcept>
class Location {
public:
    // Constructor
    Location(const std::string& root, const std::string& index);

    // Getter functions
    const std::string& getRoot() const;
    const std::string& getIndex() const;

private:
    std::string root;
    std::string index;
};

#endif // LOCATION_HPP
