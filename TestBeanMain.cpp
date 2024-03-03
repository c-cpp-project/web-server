#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include "server/WebServer.hpp"

int main(int argc, char** argv) {
  std::string configFileName;
  if (argc > 2) {
    std::cout << "[ERROR] invalid args count" << std::endl;
    return 1;
  }
  if (argc == 1) {
    std::cout << "[INFO] use default conf file ./config/default.conf"
              << std::endl;
    configFileName = "server/config/default.conf";
  } else
    configFileName = argv[1];
  ConfigParser configParser;
  configParser.parseConfig(configFileName);
  std::map<int, ServerConfiguration*> serverConfigs;
  std::map<int, Server>::iterator it = configParser.server.begin();
  while (it != configParser.server.end()) {
    int port = it->first;
    Server& server = it->second;
    ServerConfiguration* serverConfig = new ServerConfiguration(server);
    serverConfigs.insert(std::pair<int, ServerConfiguration*>(
        serverConfig->getPort(), serverConfig));
    it++;
  }
  std::cout << "======================================\n";
  Event* event;

  event = 0;                      // Event 등록을 제외한 Test
  ResponseConfig responseConfig;  // response에서 사용하는 메서드 설정
  ControllerMapping controllerMapping(serverConfigs);  // Controller
  WebServer& webServer = WebServer::getInstance(serverConfigs);

  signal(SIGPIPE, SIG_IGN);
  webServer.execute();
}
