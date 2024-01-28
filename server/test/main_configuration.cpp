#include <iostream>
#include "../TestConfiguration.hpp"

int main() {
    try {
    INginxConfiguration& configuration = TestConfiguration::getInstance();
    const Location& location = configuration.getLocation("/");
    std::cout << location.getIndex() << " " << location.getRoot() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "exception occurs " << e.what() << std::endl;
    }
}

