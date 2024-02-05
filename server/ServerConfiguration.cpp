#include "ServerConfiguration.hpp"

ServerConfiguration::ServerConfiguration(Server& server) : server(server) {
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
}

ServerConfiguration::~ServerConfiguration() {}

int ServerConfiguration::getPort() const { return server.getListen(); }

int ServerConfiguration::getKeepAliveTimeout() {
  return server.getKeepAliveTimeout();
}

const std::string ServerConfiguration::getServerName() const {
  return server.getServerName();
}

const std::string ServerConfiguration::getResourcePath(std::string& uri, std::string& method) const {
  return "";
}

const std::string ServerConfiguration::getResourcePath(std::string& uri) const {
  Location location = server.getLocations()[uri];
  std::string root = location.getRoot();
  std::string index = location.getIndex()[0];
  return root + "/" + index;
}

const std::string ServerConfiguration::getErrorpageResourcePath(
    int statusCode) const {
  return server.getErrorCodePage()[statusCode];
}

// Get the redirection path for a given URI
const std::pair<std::string, std::string>
ServerConfiguration::getRedirectionPath(const std::string& uri) const {
  return server.getLocations()[uri].getRedirectionInfo();
}

// Check if a content type is supported
bool ServerConfiguration::hasContentType(const std::string& contentType) {
  // Assuming Server class has a method or a set of supported content types
  return supportedTypes.find(contentType) != supportedTypes.end();
}

const Server& ServerConfiguration::getServer() const { return server; }