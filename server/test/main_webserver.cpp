#include "../WebServer.hpp"
#include "../TestConfiguration.hpp"
#include <iostream>

int main() {
    try {
        // WebServer 인스턴스 얻기
        INginxConfiguration& configuration = TestConfiguration::getInstance();
        WebServer& server = WebServer::getInstance(configuration);
        // WebServer 실행
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    return 0;
}
