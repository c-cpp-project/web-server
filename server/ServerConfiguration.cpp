#include "ServerConfiguration.hpp"

#include <iostream>

ServerConfiguration::ServerConfiguration() : server(NULL) {}

ServerConfiguration::ServerConfiguration(Server& server) : server(&server) {}

int ServerConfiguration::getPort() const {
  std::cout << server->getListen() << std::endl;
  return server->getListen();
}

const std::string ServerConfiguration::getServerName() const {
  return server->getServerName();
}

const std::string ServerConfiguration::getCgiPath() const {
  return server->getCgiPath();
};

const std::string ServerConfiguration::getPathByRootAndValue(
    std::string root, std::string value) const {
  if (value == "/") {
    return root;
  }
  if (root[root.length() - 1] == '/' && value[0] == '/') {
    return root + value.substr(1);
  }
  if (root[root.length() - 1] == '/') {
    return root + value;
  }
  if (value[0] == '/') {
    return root + value;
  }
  return root + '/' + value;
}

const std::string ServerConfiguration::getErrorpageResourcePath(
    int statusCode) const {
  if (!server || server->getErrorCodePage().find(statusCode) ==
                     server->getErrorCodePage().end()) {
    return "";  // 존재하지 않음
  }
  return server->getErrorCodePage()[statusCode];
}

Location* ServerConfiguration::getLocation(const std::string uri) const {
  const std::string locationUri = findLocationUri(uri);
  return server->getLocations()[locationUri];
}

const std::pair<std::string, std::string>
ServerConfiguration::getRedirectionPath(const std::string uri) const {
  const std::string locationUri = findLocationUri(uri);
  std::cout << locationUri << std::endl;
  Location* location = server->getLocations()[locationUri];
  if (!server || location == NULL) {
    return std::pair<std::string, std::string>("400", "");  // 존재하지 않음
  }
  return location->getRedirectionInfo();
}

Server* ServerConfiguration::getServer() { return server; }

const std::string ServerConfiguration::getRoot() const {
  return server->getRoot();
}

const std::string ServerConfiguration::getUploadPath() const {
  return server->getUploadPath();
}

long ServerConfiguration::getClientBodySize(const std::string& uri) const {
  Location* location = getLocation(uri);
  if (location == NULL || location->getClientBodySize() == 0)
    return server->getClientBodySize();
  else
    return location->getClientBodySize();
};
long ServerConfiguration::getClientHeaderSize() const {
  return server->getClientHeaderSize();
};
long ServerConfiguration::getClientRequestSize(const std::string& uri) const {
  return getClientBodySize(uri) + getClientHeaderSize();
};

const std::set<std::string> ServerConfiguration::getAllowedMethod(
    const std::string& uri) const {
  // 'server' 포인터와 'uri' 키의 유효성을 검증합니다.
  if (!server ||
      server->getLocations().find(uri) == server->getLocations().end()) {
    return defaultAllowedMethods;  // 기본 허용 메소드 반환
  }

  // 'uri' 키에 해당하는 Location 객체를 안전하게 접근합니다.
  const std::map<std::string, Location*>& locations = server->getLocations();
  std::map<std::string, Location*>::const_iterator it = locations.find(uri);
  if (it != locations.end()) {
    Location* location = it->second;
    return location->getAllowMethod();
  }

  // 'uri' 키가 존재하지 않는 경우, 기본 허용 메소드 세트를 반환합니다.
  return defaultAllowedMethods;
};

const std::string ServerConfiguration::getPostCgiPath() const {
  return server->getPostCgiPath();
};

const std::string ServerConfiguration::getGetCgiPath() const {
  return server->getGetCgiPath();
};
const std::string ServerConfiguration::getDeleteCgiPath() const {
  return server->getDeleteCgiPath();
};

ServerConfiguration::~ServerConfiguration(){

};

const std::string ServerConfiguration::findLocationUri(std::string uri) const {
  std::string uriPattern;
  std::map<std::string, Location*> locations = server->getLocations();
  std::map<std::string, Location*>::iterator location;
  // Location* targetLocation = NULL;
  bool matchesPattern = false;
  long slashIdx;
  std::string foundUri;
  if (!uri.empty() && uri[uri.size() - 1] != '/') uri += "/";
  uriPattern = uri;
  while (uriPattern != "/") {
    if (matchesPattern) break;
    slashIdx = uriPattern.find_last_of('/');
    if (slashIdx == std::string::npos) break;
    if (slashIdx == 0)
      uriPattern = "/";
    else
      uriPattern = uriPattern.substr(0, slashIdx);
    for (location = locations.begin(); location != locations.end();
         ++location) {
      if (uriPattern == location->first) {
        foundUri = location->first;
        matchesPattern = true;
        // targetLocation = location->second;
        break;
      }
    }
  }
  if (matchesPattern) {
    return foundUri;
  }
  if (uri[0] == '/') {
    return "/";
  }
  return "";
}

const std::string ServerConfiguration::findUriPattern(std::string uri) const {
  std::string uriPattern;
  std::map<std::string, Location*> locations = server->getLocations();
  std::map<std::string, Location*>::iterator location;
  Location* targetLocation = NULL;
  bool matchesPattern;
  long slashIdx;
  std::string foundUri;
  if (uri[uri.size() - 1] != '/') uri += "/";
  uriPattern = uri;
  while (uriPattern != "/") {
    if (matchesPattern) break;
    slashIdx = uriPattern.find_last_of('/');
    if (slashIdx == std::string::npos) break;
    if (slashIdx == 0)
      uriPattern = "/";
    else
      uriPattern = uriPattern.substr(0, slashIdx);
    for (location = locations.begin(); location != locations.end();
         ++location) {
      if (uriPattern == location->first) {
        foundUri = location->first;
        matchesPattern = true;
        targetLocation = location->second;
        break;
      }
    }
  }
  std::cout << matchesPattern << std::endl;
  if (matchesPattern) {
    std::string partial = uri.substr(slashIdx);
    partial.erase(partial.size() - 1);
    if (targetLocation->getRoot() == "") {
      return getPathByRootAndValue(
          getPathByRootAndValue(server->getRoot(), partial),
          targetLocation->getIndex()[0]);
    }
    return getPathByRootAndValue(
        getPathByRootAndValue(targetLocation->getRoot(), partial),
        targetLocation->getIndex()[0]);
  }
  if (uri == "/") {
    targetLocation = locations["/"];
    if (targetLocation->getRoot() == "") {
      return getPathByRootAndValue(server->getRoot(),
                                   targetLocation->getIndex()[0]);
    }
    return getPathByRootAndValue(targetLocation->getRoot(),
                                 targetLocation->getIndex()[0]);
  }
  return uri.erase(uri.size() - 1);
}

const std::string ServerConfiguration::getCgiTestExt() const {
  return server->getCgiTestExt();
}