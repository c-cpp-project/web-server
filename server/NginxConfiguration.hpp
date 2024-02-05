#ifndef NginxConfiguration_HPP
#define NginxConfiguration_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "Location.hpp"

class NginxConfiguration {
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
  NginxConfiguration();
  NginxConfiguration(const NginxConfiguration& ref);
  ~NginxConfiguration();
  NginxConfiguration& operator=(const NginxConfiguration& ref);
  std::string readFile(const std::string& fileName);
  void parse(std::string& content);

 public:
  static NginxConfiguration& getInstance();
  int getPort();
  bool getSendFile();
  int getKeepAliveTimeout();
  const std::string& getServerName() const;
  const Location& getLocation(const std::string& path) const;
  const Location& getErrorpageLocation(int statusCode);
  const Location& getRedirectionPath(int statusCode);
  bool hasContentType(const std::string& contentType);
};

#endif
