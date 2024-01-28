#include <iostream>
#include "../Location.hpp"

int main() {
    // Create a Location object
    Location loc("rootPath", "indexFile");

    // Output the values of root and index
    std::cout << "Root: " << loc.getRoot() << std::endl;
    std::cout << "Index: " << loc.getIndex() << std::endl;

    return 0;
}
