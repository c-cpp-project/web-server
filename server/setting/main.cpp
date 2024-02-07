#include <fstream>
#include <iostream>

// #include "Color.hpp"
#include "../ServerConfiguration.hpp"
#include "ConfigParser.hpp"
// #include "ServerManager.hpp"

int main(int argc, char** argv) {
  std::string configFileName;

  if (argc > 2) {
    std::cout << "please input only one file..." << std::endl;
    return 1;
  }
  if (argc == 1) {
    std::cout << "use default config file..." << std::endl;
    configFileName = "default.conf";
  } else
    configFileName = argv[1];
  ConfigParser configParser;
  //   ServerManager serverManager;

  configParser.parseConfig(configFileName);
  // std::cout << configParser.server[8080].getRoot() << std::endl;
  // std::cout << configParser.server[8080].getAutoIndex() << std::endl;
  // std::cout << configParser.server[8080].getKeepAliveTimeout() << std::endl;
  // std::cout << configParser.server[80].getClientBodySize() << std::endl;

  std::cout << configParser.server[80].getUploadPath() << std::endl;
  // ServerConfiguration serverConfiguration(configParser.server[80]);
  // std::cout << serverConfiguration.getRedirectionPath("/redirection").first
  //           << std::endl;
  // std::cout << serverConfiguration.getRedirectionPath("/redirection").second
  //           << std::endl;
}
