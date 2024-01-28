#include "Location.hpp"

// Constructor implementation
Location::Location(const std::string& root, const std::string& index)
    : root(root), index(index) {}

// Getter function for root
const std::string& Location::getRoot() const {
    return root;
}

// Getter function for index
const std::string& Location::getIndex() const {
    return index;
}
