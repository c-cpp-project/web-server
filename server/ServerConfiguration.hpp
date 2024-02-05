#ifndef ServerConfiguration_HPP
#define ServerConfiguration_HPP
#include <map>
#include <set>

#include "IServerConfiguration.hpp"
#include "setting/Server.hpp"

class ServerConfiguration {
 private:
  std::set<std::string> supportedTypes;
  std::map<int, int> serverSocketPortMap;
  Server& server;

 public:
  ServerConfiguration(Server& server);
  ServerConfiguration();
  ~ServerConfiguration();
  int getPort() const;
  int getKeepAliveTimeout();
  const std::string getServerName() const;
  const std::string getResourcePath(std::string& uri) const;
  const std::string getResourcePath(std::string& uri, std::string& method) const;
  const std::string getErrorpageResourcePath(int statusCode) const;
  const std::pair<std::string, std::string> getRedirectionPath(
      const std::string& uri) const;
  bool hasContentType(const std::string& contentType);
  const Server& getServer() const;
};

#endif