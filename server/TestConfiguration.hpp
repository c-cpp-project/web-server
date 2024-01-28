#ifndef TestConfiguration_HPP
#define TestConfiguration_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "Location.hpp"
#include "INginxConfiguration.hpp"

class TestConfiguration : public INginxConfiguration {
 private:
  int port;
  std::string serverName;
  std::string defaultType;
  std::set<std::string> supportedTypes;
  bool sendFile;
  int keepAliveTimeout;
  std::map<int, Location> errorPageLocation;
  std::map<int, Location> redirectionPath;
  std::map<std::string, Location> locationMap;
  TestConfiguration();
  TestConfiguration(const TestConfiguration& ref);
  ~TestConfiguration();
  TestConfiguration& operator=(const TestConfiguration& ref);

 public:
  static TestConfiguration& getInstance();
  int getPort() const;
  bool getSendFile();
  int getKeepAliveTimeout();
  const std::string& getServerName() const;
  const Location& getLocation(const std::string& path) const;
  const Location& getErrorpageLocation(int statusCode);
  const Location& getRedirectionPath(int statusCode);
  bool hasContentType(const std::string& contentType);
};

#endif
