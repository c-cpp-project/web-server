#ifndef IServerConfiguration_HPP
#define IServerConfiguration_HPP

#include <map>
#include <set>
#include <string>

class IServerConfiguration {
 public:
  virtual int getPort() const = 0;
  virtual int getKeepAliveTimeout() = 0;
  virtual const std::string& getServerName() const = 0;
  virtual const std::string& getResourcePath(std::string& uri) const = 0;
  virtual const std::string& getErrorpageResourcePath(int statusCode) const = 0;
  virtual const std::pair<std::string, std::string> getRedirectionPath(
      std::string& uri) const = 0;
  virtual bool hasContentType(const std::string& contentType) = 0;
  virtual ~IServerConfiguration() {}
};

#endif