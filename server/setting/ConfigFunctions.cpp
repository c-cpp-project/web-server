#include "ConfigParser.hpp"
#include "Location.hpp"
#include "Server.hpp"

int ConfigParser::checkServer(struct s_info& parse_info,
                              mapPortServer& servers) {
  std::string& input = parse_info.inputToken;

  static_cast<void>(servers);
  if (input == "server") return 1;
  return CONFIG_ERROR;
}

int ConfigParser::checkLeftBrace(struct s_info& parse_info,
                                 mapPortServer& servers) {
  std::string& input = parse_info.inputToken;

  static_cast<void>(servers);
  if (input == "{")
    return 2;
  else
    return CONFIG_ERROR;
}

int ConfigParser::checkServerBlock(struct s_info& parse_info,
                                   mapPortServer& servers) {
  std::string& input = parse_info.inputToken;
  std::vector<std::string>& vecInput = parse_info.vecInput;
  std::map<std::string, std::string>& mapSentence = parse_info.mapSentence;
  std::map<std::string, Location*>& locations = parse_info.locations;
  Server server;

  if (input == "}") {
    if (vecInput.size() != 0) return -1;
    if (server.fillServer(locations,
                          mapSentence))  // 잘못된 sentence가 있으면 true를 반환
      return -1;
    locations.clear();
    mapSentence.clear();
    if (servers.find(std::make_pair(server.getServerName(),
                                    server.getListen())) == servers.end()) {
      servers[std::make_pair(server.getServerName(), server.getListen())] =
          server;
      return 0;
    } else
      return -1;
  } else if (input == "location")
    return 4;
  else if (input == ";")
    return CONFIG_ERROR;
  else {
    vecInput.push_back(input);
    return 3;
  }
}

int ConfigParser::checkServerKeyAndValue(struct s_info& parse_info,
                                         mapPortServer& servers) {
  std::string& input = parse_info.inputToken;
  std::vector<std::string>& vecInput = parse_info.vecInput;
  std::map<std::string, std::string>& mapSentence = parse_info.mapSentence;
  std::string concatTokens;
  int vecInputSize = vecInput.size();

  static_cast<void>(servers);
  if (input == ";") {
    if (vecInputSize < 2)
      return CONFIG_ERROR;
    else if (vecInputSize == 2) {
      if (vecInput[0] == "return" || vecInput[0] == "error_page")
        return CONFIG_ERROR;
      mapSentence.insert(
          std::pair<std::string, std::string>(vecInput[0], vecInput[1]));
      vecInput.clear();
      return 2;
    } else {
      if (vecInput[0] != "error_page" && vecInput[0] != "return" &&
          vecInput[0] != "allow_method" && vecInput[0] != "index")
        return CONFIG_ERROR;
      concatTokens = "";
      for (int i = 1; i < vecInputSize; i++) {
        concatTokens += vecInput[i] += " ";
      }
      mapSentence.insert(
          std::pair<std::string, std::string>(vecInput[0], concatTokens));
      vecInput.clear();
      return 2;
    }
  } else if (input == "{" || input == "}")
    return CONFIG_ERROR;
  else {
    vecInput.push_back(input);
    return 3;
  }
}

int ConfigParser::checkLocationBlock(struct s_info& parse_info,
                                     mapPortServer& servers) {
  std::string& input = parse_info.inputToken;
  std::string& locationDir = parse_info.locationDir;

  static_cast<void>(servers);
  if (input == "{" || input == "}" || input == ";")
    return CONFIG_ERROR;
  else {
    locationDir = input;
    return 5;
  }
}

int ConfigParser::checkLocationLeftBrace(struct s_info& parse_info,
                                         mapPortServer& servers) {
  std::string& input = parse_info.inputToken;

  static_cast<void>(servers);
  if (input == "{")
    return 6;
  else
    return CONFIG_ERROR;
}

int ConfigParser::checkLocationKeyAndValue(struct s_info& parse_info,
                                           mapPortServer& servers) {
  std::string& input = parse_info.inputToken;
  std::vector<std::string>& vecInput = parse_info.vecInput;
  std::map<std::string, std::string>& mapSentence = parse_info.mapSentence;
  std::string& locationDir = parse_info.locationDir;
  std::map<std::string, Location*>& locations = parse_info.locations;
  Location* locationBlock = new Location();

  static_cast<void>(servers);
  if (input == "}") {
    if (vecInput.size() != 0) return CONFIG_ERROR;
    if (locationBlock->fillLocationBlock(mapSentence)) {
      delete locationBlock;
      return CONFIG_ERROR;
    }
    mapSentence.clear();
    locations.insert(
        std::pair<std::string, Location*>(locationDir, locationBlock));
    return 2;
  } else if (input == ";" || input == "{") {
    delete locationBlock;
    return CONFIG_ERROR;
  } else {
    vecInput.push_back(input);
    delete locationBlock;
    return 7;
  }
}

int ConfigParser::checkLocationElement(struct s_info& parse_info,
                                       mapPortServer& servers) {
  std::string& input = parse_info.inputToken;
  std::vector<std::string>& vecInput = parse_info.vecInput;
  std::map<std::string, std::string>& mapSentence = parse_info.mapSentence;
  std::string concatTokens;
  int vecInputSize = vecInput.size();

  static_cast<void>(servers);
  if (input == ";") {
    if (vecInputSize < 2)
      return CONFIG_ERROR;
    else if (vecInputSize == 2) {
      if (vecInput[0] == "return " || vecInput[0] == "error_page")
        return CONFIG_ERROR;
      mapSentence.insert(
          std::pair<std::string, std::string>(vecInput[0], vecInput[1]));
      vecInput.clear();
      return 6;
    } else {
      if (vecInput[0] != "error_page" && vecInput[0] != "return" &&
          vecInput[0] != "allow_method" && vecInput[0] != "index")
        return CONFIG_ERROR;
      concatTokens = "";
      for (int i = 1; i < vecInputSize; i++) {
        concatTokens += vecInput[i] += " ";
      }
      mapSentence.insert(
          std::pair<std::string, std::string>(vecInput[0], concatTokens));
      vecInput.clear();
      return 6;
    }
  } else if (input == "{" || input == "}")
    return CONFIG_ERROR;
  else {
    vecInput.push_back(input);
    return 7;
  }
}
