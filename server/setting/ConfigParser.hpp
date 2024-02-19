#pragma once

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Server.hpp"
#define CONFIG_ERROR -1
enum METHOD {
  GET = 1,
  POST = 2,
  PUT = 4,
  DELETE = 8,
  HEAD = 16,
};

class Location;
class Server;
class ConfigParser {
 public:
  std::map<int, Server> server;
  void parseConfig(std::string const& configFileName);
  ~ConfigParser();
  ConfigParser();

 private:
  typedef std::map<int, Server> mapPortServer;
  typedef std::map<std::string, Location*> mapStrLocation;
  typedef std::vector<std::string> vecStr;
  typedef std::map<std::string, std::string> mapStrStr;

  ConfigParser(ConfigParser const& other);
  ConfigParser& operator=(ConfigParser const& rhs);

  struct s_info {
    std::string inputToken;
    std::map<std::string, Location*> locations;
    std::string locationDir;
    std::vector<std::string> vecInput;
    std::map<std::string, std::string> mapSentence;
  };

  static int checkServer(struct s_info& parse_info, mapPortServer& servers);
  static int checkLeftBrace(struct s_info& parse_info, mapPortServer& servers);
  static int checkServerBlock(struct s_info& parse_info,
                              mapPortServer& servers);
  static int checkServerKeyAndValue(struct s_info& parse_info,
                                    mapPortServer& servers);
  static int checkLocationBlock(struct s_info& parse_info,
                                mapPortServer& servers);
  static int checkLocationLeftBrace(struct s_info& parse_info,
                                    mapPortServer& servers);
  static int checkLocationKeyAndValue(struct s_info& parse_info,
                                      mapPortServer& servers);
  static int checkLocationElement(struct s_info& parse_info,
                                  mapPortServer& servers);
};
#endif