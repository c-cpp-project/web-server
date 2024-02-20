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
  std::string cgiPath;
  std::string _getCgiPath;
  std::string postCgiPath;
  std::string deleteCgiPath;
  std::map<std::vector<int>, std::string> errorPage;
  std::map<int, std::string> errorCodePage;
  std::map<std::string, int> redirection;
  bool autoIndex;
  long clientBodySize;
  long clientHeaderSize;
  std::map<std::string, Location*> locations;
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
  void setClientHeaderSize(std::string& input);
  void setCgiPath(std::string& input);

 public:
  Server();
  ~Server();
  bool fillServer(std::map<std::string, Location*>& mapLocations,
                  std::map<std::string, std::string>& mapSentence);

  int getListen() const;
  std::string getRoot() const;
  std::map<std::vector<int>, std::string> getErrorPage() const;
  std::map<int, std::string> getErrorCodePage();
  std::map<std::string, int> getRedirection() const;
  bool getAutoIndex() const;
  long getClientBodySize() const;
  long getClientHeaderSize() const;
  std::map<std::string, Location*>& getLocations();
  std::string getServerName() const;
  std::string getUploadPath() const;
  std::string getCgiPath() const;
  int getKeepAliveTimeout() const;
  std::string getGetCgiPath() const;
  std::string getPostCgiPath() const;
  std::string getDeleteCgiPath() const;
  void setGetCgiPath(std::string& input);
  void setPostCgiPath(std::string& input);
  void setDeleteCgiPath(std::string& input);
};
