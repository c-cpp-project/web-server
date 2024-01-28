#include "TestConfiguration.hpp"

TestConfiguration::TestConfiguration()
    : port(80), serverName("localhost"), defaultType("text/html"),
      sendFile(false), keepAliveTimeout(60) {
    locationMap.insert(std::make_pair("/", Location("/usr/share/nginx/html", "index.html")));
    locationMap.insert(std::make_pair("/images", Location("/usr/share/nginx/images", "index.html")));
}

TestConfiguration::TestConfiguration(const TestConfiguration& ref)
    : port(ref.port), serverName(ref.serverName), defaultType(ref.defaultType),
      supportedTypes(ref.supportedTypes), sendFile(ref.sendFile),
      keepAliveTimeout(ref.keepAliveTimeout), errorPageLocation(ref.errorPageLocation),
      redirectionPath(ref.redirectionPath), locationMap(ref.locationMap) {
}

// Destructor
TestConfiguration::~TestConfiguration() {
    // 리소스 해제 코드를 작성하세요.
}

// Assignment Operator
TestConfiguration& TestConfiguration::operator=(const TestConfiguration& ref) {
    if (this != &ref) {
        port = ref.port;
        serverName = ref.serverName;
        defaultType = ref.defaultType;
        supportedTypes = ref.supportedTypes;
        sendFile = ref.sendFile;
        keepAliveTimeout = ref.keepAliveTimeout;
        errorPageLocation = ref.errorPageLocation;
        redirectionPath = ref.redirectionPath;
        locationMap = ref.locationMap;
    }
    return *this;
}

// Singleton Instance
TestConfiguration& TestConfiguration::getInstance() {
    static TestConfiguration instance;
    return instance;
}

// Getters Implementation
int TestConfiguration::getPort() const {
    return port;
}

bool TestConfiguration::getSendFile() {
    return sendFile;
}

int TestConfiguration::getKeepAliveTimeout() {
    return keepAliveTimeout;
}

const std::string& TestConfiguration::getServerName() const {
    return serverName;
}

const Location& TestConfiguration::getLocation(const std::string& path) const {
    std::map<std::string, Location>::const_iterator it = locationMap.find(path);
    if (it == locationMap.end()) {
        throw std::runtime_error("location 오류 발생!");
    }
    return it->second;
}

const Location& TestConfiguration::getErrorpageLocation(int statusCode) {
    std::map<int, Location>::const_iterator it = errorPageLocation.find(statusCode);
    if (it == errorPageLocation.end()) {
        throw std::runtime_error("errorPageLocation 가 없습니다.");
    }
    return it->second;
}

const Location& TestConfiguration::getRedirectionPath(int statusCode) {
    std::map<int, Location>::const_iterator it = redirectionPath.find(statusCode);
    if (it == redirectionPath.end()) {
        throw std::runtime_error("redirectionPath 가 없습니다.");
    }
    return it->second;
}

bool TestConfiguration::hasContentType(const std::string& contentType) {
    return supportedTypes.find(contentType) != supportedTypes.end();
}
