#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

class Location {
 private:
  std::string root;
  std::map<std::vector<int>, std::string> errorPage;
  std::map<int, std::string> errorCodePage;
  std::map<std::string, int> redirection;
  std::set<std::string> allowedMethods;
  std::vector<std::string> index;
  bool autoIndex;
  size_t clientBodySize;
  std::pair<std::string, std::string> redirectionInfo;
  // setter
  void setRoot(std::string& input);
  bool setErrorPage(std::string const& sentence);
  bool setRedirection(std::string const& sentence);
  void setAutoIndex(std::string& input);
  void setClientBodySize(std::string& input);
  bool setAllowMethod(std::string& sentence);
  void setIndex(std::string& sentence);

 public:
  Location();
  ~Location();
  bool fillLocationBlock(std::map<std::string, std::string>& mapSentence);
  std::pair<std::string, std::string> getRedirectionInfo() const;
  // getter
  std::string getRoot(void) const;
  std::map<std::vector<int>, std::string> getErrorPage() const;
  std::map<int, std::string> getErrorCodePage() const;
  std::map<std::string, int> getRedirection() const;
  const std::set<std::string> getAllowMethod() const;
  std::vector<std::string> getIndex() const;
  bool getAutoIndex() const;
  size_t getClientBodySize() const;
};
