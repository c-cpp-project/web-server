#pragma once

#include <map>
#include <string>

#include "Location.hpp"

class Server {
 private:
  int listen;
  int keepAliveTimeout;
  std::string serverName;
  std::string root;
  std::map<std::vector<int>, std::string> errorPage;
  std::map<int, std::string> errorCodePage;
  std::map<std::string, int> redirection;
  bool autoIndex;
  size_t clientBodySize;
  std::map<std::string, Location> locations;
  std::string uploadedPath;

  void setListen(std::string& input);
  void setServerName(std::string& input);
  void setRoot(std::string& input);
  bool setErrorPage(std::string const& sentence);
  bool setRedirection(std::string const& sentence);
  void setAutoIndex(std::string& input);
  void setClientBodySize(std::string& input);
  void setUploadPath(std::string& input);
  void setKeepAliveTimeout(std::string& input);

 public:
  Server();
  ~Server();
  bool fillServer(std::map<std::string, Location>& mapLocations,
                  std::map<std::string, std::string>& mapSentence);

  int getListen() const;
  std::string getRoot() const;
  std::map<std::vector<int>, std::string> getErrorPage() const;
  std::map<int, std::string> getErrorCodePage();
  std::map<std::string, int> getRedirection() const;
  bool getAutoIndex() const;
  unsigned int getClientBodySize() const;
  std::map<std::string, Location> getLocations() const;
  std::string getServerName() const;
  std::string getUploadPath() const;
  int getKeepAliveTimeout() const;
};
