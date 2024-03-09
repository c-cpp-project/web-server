#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include "server/WebServer.hpp"

int main(int argc, char** argv) {
  int option;
  std::string configFileName;
  if (argc > 2) {
    std::cout << "[ERROR] invalid args count" << std::endl;
    return 1;
  }
  if (argc == 1) {
    std::cout << "[INFO] use default conf file ./config/default.conf"
              << std::endl;
    configFileName = "server/config/default.conf";
    option = 0;
  } else {
    configFileName = argv[1];
    option = 10;
  }
  ConfigParser configParser;
  configParser.parseConfig(configFileName);
  std::map<std::pair<std::string, int>, ServerConfiguration*> serverConfigs;
  std::map<std::pair<std::string, int>, Server>::iterator it =
      configParser.server.begin();
  while (it != configParser.server.end()) {
    std::string serverName = it->first.first;
    int port = it->first.second;
    Server& server = it->second;
    ServerConfiguration* serverConfig = new ServerConfiguration(server);
    serverConfigs.insert(std::make_pair(
        std::make_pair(server.getServerName(), server.getListen()),
        serverConfig));
    it++;
  }
  std::cout << "======================================\n";
  Event* event;

  event = 0;                      // Event 등록을 제외한 Test
  ResponseConfig responseConfig;  // response에서 사용하는 메서드 설정
  ControllerMapping controllerMapping(serverConfigs);  // Controller
  WebServer& webServer = WebServer::getInstance(serverConfigs, option);

  signal(SIGPIPE, SIG_IGN);
  webServer.execute();
}
