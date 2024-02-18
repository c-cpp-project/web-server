#include "ServerConfiguration.hpp"
#include <iostream>

ServerConfiguration::ServerConfiguration() : server(NULL) {}

ServerConfiguration::ServerConfiguration(Server& server) : server(&server) {
  this->supportedTypes.insert("text/html");
  this->supportedTypes.insert("text/plain");
  this->supportedTypes.insert("text/css");
  this->supportedTypes.insert(
      "text/javascript");  // 또는 application/javascript

  // 애플리케이션 타입
  this->supportedTypes.insert("application/json");
  this->supportedTypes.insert("application/xml");
  this->supportedTypes.insert("application/x-www-form-urlencoded");
  this->supportedTypes.insert("application/pdf");
  this->supportedTypes.insert("application/octet-stream");

  // 이미지 타입
  this->supportedTypes.insert("image/jpeg");
  this->supportedTypes.insert("image/png");
  this->supportedTypes.insert("image/gif");
  this->supportedTypes.insert("image/webp");

  // 오디오/비디오 타입
  this->supportedTypes.insert("audio/mpeg");
  this->supportedTypes.insert("audio/ogg");
  this->supportedTypes.insert("video/mp4");
  this->supportedTypes.insert("video/webm");
  this->defaultAllowedMethods.insert("GET");
}

int ServerConfiguration::getPort() const { 
  std::cout << server->getListen() << std::endl;
  return server->getListen(); 
}

int ServerConfiguration::getKeepAliveTimeout() {
  return server->getKeepAliveTimeout();
}

const std::string ServerConfiguration::getServerName() const {
  return server->getServerName();
}

const std::string ServerConfiguration::getCgiPath() const {
  return server->getCgiPath();
};

const std::string ServerConfiguration::getResourcePath(std::string uri) const {
  Location location = server->getLocations()[uri];
  std::vector<std::string> indexVectors = location.getIndex();
  if (server->getLocations().find(uri) == server->getLocations().end() || location.getIndex().size() == 0){
    return getPathByRootAndValue(server->getRoot(), uri);
  }
  if (location.getRoot() == "") {
    return getPathByRootAndValue(server->getRoot(), indexVectors[0]);
  }
  return getPathByRootAndValue(location.getRoot(), indexVectors[0]);
}

const std::string ServerConfiguration::getPathByRootAndValue(std::string root, std::string value ) const {
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
  if (!server || server->getErrorCodePage().find(statusCode) == server->getErrorCodePage().end()) {
    return "";// 존재하지 않음
  }
  return server->getErrorCodePage()[statusCode];
}

const std::pair<std::string, std::string>
ServerConfiguration::getRedirectionPath(const std::string& uri) const {
  if (!server || server->getLocations().find(uri) == server->getLocations().end()) {
    return std::pair<std::string, std::string>("400", ""); // 존재하지 않음
  }
  return server->getLocations()[uri].getRedirectionInfo();
}

bool ServerConfiguration::hasContentType(const std::string& contentType) {
  return supportedTypes.find(contentType) != supportedTypes.end();
}

const Server& ServerConfiguration::getServer() const { return *server; }

const std::string ServerConfiguration::getRoot() const {
  return server->getRoot();
}

const std::string ServerConfiguration::getUploadPath() const {
  return server->getUploadPath();
}

long ServerConfiguration::getClientBodySize() const {
  return server->getClientBodySize();
};
long ServerConfiguration::getClientHeaderSize() const {
  return server->getClientHeaderSize();
};
long ServerConfiguration::getClientRequestSize() const {
  return getClientBodySize() + getClientHeaderSize();
};

const std::set<std::string> ServerConfiguration::getAllowedMethod(const std::string& uri) const {
  // 'server' 포인터와 'uri' 키의 유효성을 검증합니다.
  if (!server || server->getLocations().find(uri) == server->getLocations().end()) {
    return defaultAllowedMethods; // 기본 허용 메소드 반환
  }

  // 'uri' 키에 해당하는 Location 객체를 안전하게 접근합니다.
  const std::map<std::string, Location>& locations = server->getLocations();
  std::map<std::string, Location>::const_iterator it = locations.find(uri);
  if (it != locations.end()) {
    const Location& location = it->second;
    return location.getAllowMethod();
  }

  // 'uri' 키가 존재하지 않는 경우, 기본 허용 메소드 세트를 반환합니다.
  return defaultAllowedMethods;
}
