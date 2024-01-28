#ifndef INginxConfiguration_HPP
#define INginxConfiguration_HPP

#include <string>
#include <map>
#include <set>
#include "Location.hpp"

class INginxConfiguration {
public:
    virtual int getPort() const = 0;
    virtual bool getSendFile() = 0;
    virtual int getKeepAliveTimeout() = 0;
    virtual const std::string& getServerName() const = 0;
    virtual const Location& getLocation(const std::string& path) const = 0;
    virtual const Location& getErrorpageLocation(int statusCode) = 0;
    virtual const Location& getRedirectionPath(int statusCode) = 0;
    virtual bool hasContentType(const std::string& contentType) = 0;
    virtual ~INginxConfiguration() {}
};

#endif
