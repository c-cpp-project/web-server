#include "./setting/ConfigParser.hpp"
#include "ServerConfiguration.hpp"
#include "WebServer.hpp"

std::map<int, ServerConfiguration*> wrapServer(std::map<int, Server> servers, std::map<int, ServerConfiguration*> maps) {
  std::map<int, Server>::iterator it = servers.begin();
  while (it != servers.end()) {
    int port = it->first;
    Server& server = it->second;
    ServerConfiguration* serverConfig = new ServerConfiguration(server);
    maps.insert(std::pair<int, ServerConfiguration*>(serverConfig->getPort(),serverConfig));
    std::cout << "server2 : " << maps[serverConfig->getPort()]->getPort() << std::endl;
    std::cout << "server2 : " << maps[serverConfig->getPort()]->getAllowedMethod("/").size() << std::endl;
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
  std::map<int, ServerConfiguration*> serverConfigs;
  std::map<int, Server>::iterator it = configParser.server.begin();
  while (it != configParser.server.end()) {
    int port = it->first;
    Server& server = it->second;
    ServerConfiguration* serverConfig = new ServerConfiguration(server);
    serverConfigs.insert(std::pair<int, ServerConfiguration*>(serverConfig->getPort(),serverConfig));
    it++;
  }
  WebServer& webServer = WebServer::getInstance(serverConfigs);
  // webServer.execute();
  std::cout << serverConfigs[80]->getRedirectionPath("/ddd").first << std::endl;
  std::cout << serverConfigs[8080]->getPort() << std::endl;

  std::set<std::string> sets = serverConfigs[80]->getAllowedMethod("/ddd");
}
