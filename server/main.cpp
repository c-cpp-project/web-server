#include "./setting/ConfigParser.hpp"
#include "ServerConfiguration.hpp"
#include "WebServer.hpp"

std::map<int, ServerConfiguration> wrapServer(std::map<int, Server> servers) {
  std::map<int, ServerConfiguration> maps;
  std::map<int, Server>::iterator it = servers.begin();
  while (it != servers.end()) {
    int port = it->first;
    Server& server = it->second;
    const ServerConfiguration& serverConfig = ServerConfiguration(server);
    maps.insert(std::pair<int, ServerConfiguration>(port, serverConfig));
    it++;
  }
  return maps;
}

int main(int argc, char** argv) {
  std::string configFileName;

  if (argc > 2) {
    std::cout << "[ERROR] invalid args count" << std::endl;
    return 1;
  }
  if (argc == 1) {
    std::cout << "[INFO] use default conf file ./config/default.conf"
              << std::endl;
    configFileName = "./config/default.conf";
  } else
    configFileName = argv[1];
  ConfigParser configParser;
  //   ServerManager serverManager;

  configParser.parseConfig(configFileName);
  WebServer& webServer =
      WebServer::getInstance(wrapServer(configParser.server));
  webServer.execute();
  // std::cout << configParser.server[8080].getRoot() << std::endl;
  // std::cout << configParser.server[8080].getAutoIndex() << std::endl;
  // std::cout << configParser.server[8080].getKeepAliveTimeout() << std::endl;
  // std::cout << configParser.server[80].getClientBodySize() << std::endl;

  // std::cout << configParser.server[80].getErrorCodePage()[403] << std::endl;
  // ServerConfiguration serverConfiguration(configParser.server[80]);
  // // std::cout <<
  // serverConfiguration.getRedirectionPath("/redirection").first
  // //           << std::endl;
  // // std::cout <<
  // serverConfiguration.getRedirectionPath("/redirection").second
  // //           << std::endl;
}