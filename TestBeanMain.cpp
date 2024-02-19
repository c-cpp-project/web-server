#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

// #include"ResponseHandler/HttpResponse.hpp"
#include "HttpRequest/HttpHeaders.hpp"
#include "HttpRequest/HttpRequest.hpp"
#include "HttpRequest/HttpRequestHandler.hpp"
#include "HttpRequest/HttpRequestParser.hpp"
#include "HttpRequest/RequestLine.hpp"
#include "HttpRequest/RequestParams.hpp"
#include "HttpRequest/RequestUtility.hpp"
#include "HttpRequest/SocketCloseException.hpp"
#include "ResponseConfig.hpp"
#include "ResponseHandler/FrontController.hpp"
#include "ResponseHandler/MultiRequest.hpp"
#include "controllers/Controller.hpp"
#include "controllers/ControllerMapping.hpp"
#include "controllers/MyController.hpp"
#include "server/ServerConfiguration.hpp"
#include "server/setting/ConfigParser.hpp"
#include "server/setting/Server.hpp"

// Bean 설정
#include "Bean/Bean.hpp"
#include "Bean/BeanFactory.hpp"
#include "Bean/HttpHandler.hpp"
#include "Bean/ReadEventBean.hpp"
#include "Bean/RecvEventBean.hpp"
#include "Bean/SendEventBean.hpp"
#include "Bean/WriteEventBean.hpp"

int tcp_listening(int& server_socket) {
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Error create socket\n";
    return (-1);
  }

  // 소켓 구조체 생성, 소켓과 주소를 바인딩
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(80);
  if (bind(server_socket, (struct sockaddr*)&server_address,
           sizeof(server_address)) == -1) {
    std::cerr << "Error binding socket\n";
    close(server_socket);
    return (-1);
  }

  // 연결 대기열 설정
  if (listen(server_socket, 5) == -1) {
    std::cerr << "Error setting up listen queue\n";
    close(server_socket);
    return (-1);
  }

  std::cout << "Server is listening on port 12345...\n";
  return (1);
}

int tcp_connection(int& clientSocket, int& server_socket) {
  // 클라이언트 연결 대기
  clientSocket = accept(server_socket, NULL, NULL);
  if (clientSocket == -1) {
    std::cerr << "Error accepting client connection\n";
    close(server_socket);
    return (-1);
  }
  std::cout << "Client connected!\n";
  return (clientSocket);
}

// 주의사항: 메모리 릭
// 합의사항: Event 처리할 때, Error가 발생하면 어디서 처리할까?
// 사용법: beanFactory.runBeanByName("처리해야할 이벤트명", *uData, event);
void BeanTest(int socketfd, ServerConfiguration* serverConfig, Event* event) {
  int fd;
  BeanFactory beanFactory;

  // HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
  // beanFactory.runBeanByName("RECV", handler, event);
  std::cout << "BeanTest===================\n";

  event = 0;
  beanFactory.runBeanByName("RECV", new HttpHandler(socketfd, serverConfig),
                            event);
  // 1. error handle / response.forward / GET, DELETE	 -> READ 등록
  // 2. redirect
  // -> SEND 등록
  // 3. WRITE
  // -> WRITE 등록, READ 등록
  beanFactory.runBeanByName("READ", new HttpHandler(fd, serverConfig), event);
  // 1. SEND BODY
  // -> SEND 등록
  beanFactory.runBeanByName("WRITE", new HttpHandler(fd, serverConfig), event);
  // 1. CGI파일에 WRITE
  beanFactory.runBeanByName("SEND", new HttpHandler(socketfd, serverConfig),
                            event);
  // 1. SEND
}

int endpointTest(ServerConfiguration* serverConfig, Event* event) {
  int clientSocket;
  int server_socket;

  tcp_listening(server_socket);
  clientSocket = tcp_connection(clientSocket, server_socket);
  if (clientSocket == -1) return (-1);

  // ================================= Event Test ==============================
  // //
  BeanTest(clientSocket, serverConfig, event);
  // ================================= Event Test ==============================
  // //
  close(clientSocket);
  close(server_socket);
  return (0);
}

std::map<int, ServerConfiguration*> wrapServer(std::map<int, Server> servers) {
  std::map<int, ServerConfiguration*> maps;
  std::map<int, Server>::iterator it = servers.begin();
  while (it != servers.end()) {
    int port = it->first;
    Server& server = it->second;
    ServerConfiguration* serverConfig = new ServerConfiguration(server);
    maps.insert(std::pair<int, ServerConfiguration*>(serverConfig->getPort(),
                                                     serverConfig));
    std::cout << "server2 : " << maps[serverConfig->getPort()]->getPort()
              << std::endl;
    it++;
  }
  return maps;
}

int test1(std::map<int, Server>& server) {
  std::map<int, Server>::iterator it = server.begin();
  while (it != server.end()) {
    int port = it->first;
    std::map<std::string, Location*>::iterator configIter;
    Server& server = it->second;
    for (configIter = server.getLocations().begin();
         configIter != server.getLocations().end(); configIter++) {
      std::string locationUri = configIter->first;
      const std::set<std::string> allowMethod =
          configIter->second->getAllowMethod();
      unsigned int method;

      method = 0;
      if (allowMethod.find("GET") != allowMethod.end()) method += 1;
      if (allowMethod.find("POST") != allowMethod.end()) method += 2;
      if (allowMethod.find("DELETE") != allowMethod.end()) method += 8;
      //   putController(port, locationUri, new MyController(method));
    }
    it++;
  }
  return 0;
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
  // =======================================================================================
  // //
  //											Response
  // 설정 //
  // =======================================================================================
  // //
  std::cout << "======================================\n";
  Event* event;

  event = 0;                      // Event 등록을 제외한 Test
  ResponseConfig responseConfig;  // response에서 사용하는 메서드 설정
  ControllerMapping controllerMapping(serverConfigs);  // Controller
  //  설정
  //     endpointTest(serverConfigs[80], event);
}
