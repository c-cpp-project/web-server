#include "ConfigParser.hpp"

#include <fstream>

#include "Location.hpp"
#include "Server.hpp"

// constructors
ConfigParser::ConfigParser() {}
// destructor
ConfigParser::~ConfigParser() {}
// copy constructors

void ConfigParser::parseConfig(std::string const& configFileName) {
  int (*action[8])(struct s_info&, mapPortServer&) = {checkServer,
                                                      checkLeftBrace,
                                                      checkServerBlock,
                                                      checkServerKeyAndValue,
                                                      checkLocationBlock,
                                                      checkLocationLeftBrace,
                                                      checkLocationKeyAndValue,
                                                      checkLocationElement};
  std::fstream fileStream;
  std::stringstream sstream;
  char c;
  int state;
  std::map<std::string, std::string> tmpSentence;
  int prev_state;
  std::string prev_token;
  struct s_info parse_info;

  fileStream.open(configFileName.c_str(), std::ios::in);
  if (fileStream.fail()) {
    std::cout << "[ERROR] failed to open config file" << std::endl;
    exit(1);
  }
  while (fileStream.get(c)) sstream << c;
  state = 0;
  while (sstream >> parse_info.inputToken) {
    prev_state = state;
    state = action[state](parse_info, this->server);
    if (state == CONFIG_ERROR) {
      std::cout << "[ERROR] error in token : " << prev_token << std::endl;
      break;
    }
    if (prev_state == 2 && state == 4) {
      tmpSentence = parse_info.mapSentence;
      parse_info.mapSentence.clear();
    } else if (prev_state == 6 && state == 2) {
      parse_info.mapSentence = tmpSentence;
      tmpSentence.clear();
    }
    prev_token = parse_info.inputToken;
  }
  if (state != 0) {
    std::cout << "[ERROR] config parse error occurs" << std::endl;
    exit(1);
  }
  std::cout << "[INFO] config parse success" << std::endl;
}
