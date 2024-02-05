#include "HttpRequest/HttpRequestHandler.hpp"
#include "server/ServerConfiguration.hpp"
#include "server/setting/ConfigParser.hpp"

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
    std::cout << "[INFO] use default conf file" << std::endl;
    configFileName = "server/config/default.conf";
  } else
    configFileName = argv[1];
  ConfigParser configParser;
  //   ServerManager serverManager;

  configParser.parseConfig(configFileName);
  std::map<int, ServerConfiguration> serverConfigs =
      wrapServer(configParser.server);
  ServerConfiguration serverConfig = serverConfigs[80];
  HttpRequestHandler handler = HttpRequestHandler(0, &serverConfig);
}