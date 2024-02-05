#include "WebServer.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <vector>

#include "StringUtils.hpp"

WebServer& WebServer::getInstance(
    std::map<int, ServerConfiguration> serverConfig) {
  static WebServer instance(serverConfig);
  return instance;
}

WebServer::WebServer(std::map<int, ServerConfiguration> serverConfigs) {
  this->serverConfigs = serverConfigs;
}

void WebServer::segSignalHandler(int signo) {
  static_cast<void>(signo);
  std::cout << "Segmentation Fault is detected" << std::endl;
  std::cout << "your configuration is wrong" << std::endl;
  exit(1);
}

void WebServer::init() {
  std::map<int, ServerConfiguration>::iterator it = serverConfigs.begin();
  signal(SIGSEGV, WebServer::segSignalHandler);
  int port = 8080;
  if (eventHandler.initKqueue()) {
    std::cout << "kqueue() error" << std::endl;
    exit(1);
  }
  while (it != serverConfigs.end()) {
    ServerConfiguration& serverConfig = it->second;
    int serversSocket = openPort(serverConfig);
    fcntl(serversSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    serverSocketPortMap[serversSocket] = it->first;
    std::cout << it->first << std::endl;
    it++;
  }
}

int WebServer::openPort(ServerConfiguration& serverConfig) {
  struct addrinfo* info;
  struct addrinfo hint;
  struct sockaddr_in socketaddr;
  int opt = 1;
  int port = serverConfig.getPort();
  std::string serverName = serverConfig.getServerName();

  std::cout << "[INFO] " << serverName << " "
            << "Port number : " << port << std::endl;

  memset(&hint, 0, sizeof(struct addrinfo));
  memset(&socketaddr, 0, sizeof(struct sockaddr_in));
  socketaddr.sin_family = AF_INET;
  socketaddr.sin_port = htons(port);
  socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  std::string portStr = StringUtils::toString(port);
  int res = getaddrinfo(serverName.c_str(), portStr.c_str(), &hint, &info);
  if (res == -1)
    SocketUtils::exitWithPerror("[Error] getaddrinfo() error\n" +
                                std::string(strerror(errno)));
  int serverSocket =
      socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (serverSocket == -1)
    SocketUtils::exitWithPerror("[Error] socket() error\n" +
                                std::string(strerror(errno)));
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  res = bind(serverSocket, reinterpret_cast<struct sockaddr*>(&socketaddr),
             sizeof(socketaddr));
  if (res == -1) {
    SocketUtils::exitWithPerror("[Error] bind() error\n" +
                                std::string(strerror(errno)));
  }
  res = listen(serverSocket, LISTENCAPACITY);
  if (res == -1) {
    SocketUtils::exitWithPerror("bind() error\n" +
                                std::string(strerror(errno)));
  }
  return serverSocket;
}

void WebServer::execute() {
  init();
  handleEvent();
}

void WebServer::handleEvent() {
  int newEventCount;
  while (true) {
    newEventCount = eventHandler.create();
    if (newEventCount == -1) {
      SocketUtils::exitWithPerror("event create error\n" +
                                  std::string(strerror(errno)));
    }
    eventHandler.clearChangedEventList();
    for (int i = 0; i < newEventCount; i++) {
      processEvent(eventHandler[i]);
    }
    // clientManager.clearClients();
  }
}

void WebServer::processEvent(struct kevent& currEvent) {
  if (currEvent.flags & EV_ERROR) {
    processErrorEvent(currEvent);
    return;
  }
  switch (currEvent.filter) {
    case EVFILT_READ:
      processReadEvent(currEvent);
      break;
    case EVFILT_WRITE:
      //   writeEventProcess(currEvent);
      break;
    case EVFILT_TIMER:
      //   timerEventProcess(currEvent);
      break;
  }
}

void WebServer::processErrorEvent(struct kevent& currEvent) {
  if (isValidFd(currEvent)) {
    disconnectPort(currEvent);
    std::cout << currEvent.ident << "[INFO] server disconnected" << std::endl;
  } else {
    // clientManager.addToDisconnectClient(currEvent.ident);
    std::cout << currEvent.ident << "[INFO] client disconnected" << std::endl;
  }
}

void WebServer::processReadEvent(struct kevent& currEvent) {}

int WebServer::acceptClient(int serverSocket) {
  struct _linger linger;
  linger.l_onoff = 1;
  linger.l_linger = 0;
  const int clientSocket = accept(serverSocket, NULL, NULL);
  const int serverPort = serverSocketPortMap[serverSocket];
  setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, &linger, sizeof(_linger));
  if (clientSocket == -1) {
    std::cout << "accept() error" << std::endl;
    return -1;
  }
  fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
  //   clientManager.addNewClient(client_fd, &servers[serverPort], &events);
  //   eventHandler.change(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
  //                       &clientManager.getClient(clientSocket));
  //   eventHandler.change(clientSocket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0,
  //   0,
  //                       &clientManager.getClient(clientSocket));
  return clientSocket;
}

bool WebServer::isValidFd(struct kevent& currEvent) {
  return serverSocketPortMap.find(currEvent.ident) != serverSocketPortMap.end();
}

void WebServer::disconnectPort(struct kevent& currEvent) {
  close(currEvent.ident);
  serverConfigs.erase(serverSocketPortMap[currEvent.ident]);
}

// void WebServer::run() {
//   int server_socket;
//   struct sockaddr_in server_addr;
//   // init server socket and listen
//   if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
//     SocketUtils::exitWithPerror("socket() error\n" +
//                                 std::string(strerror(errno)));
//   memset(&server_addr, 0, sizeof(server_addr));
//   server_addr.sin_family = AF_INET;
//   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//   server_addr.sin_port = htons(nginxConfig.getPort());
//   if (bind(server_socket, (struct sockaddr*)&server_addr,
//            sizeof(server_addr)) == -1)
//     SocketUtils::exitWithPerror("bind() error\n" +
//                                 std::string(strerror(errno)));

//   if (listen(server_socket, 5) == -1)
//     SocketUtils::exitWithPerror("listen() error\n" +
//                                 std::string(strerror(errno)));
//   fcntl(server_socket, F_SETFL, O_NONBLOCK);

//   int kq;
//   if ((kq = kqueue()) == -1)
//     SocketUtils::exitWithPerror("kqueue() error\n" +
//                                 std::string(strerror(errno)));
//   std::map<int, std::string> clients;      // map for client socket:data
//   std::vector<struct kevent> change_list;  // kevent vector for changelist
//   struct kevent event_list[8];             // kevent array for eventlist

//   /* add event for server socket */
//   SocketUtils::changeEvents(change_list, server_socket, EVFILT_READ,
//                             EV_ADD | EV_ENABLE, 0, 0, NULL);
//   std::cout << "web server start!" << std::endl;

//   /* main loop */
//   int new_events;
//   struct kevent* curr_event;
//   while (1) {
//     /*  apply changes and return new events(pending events) */
//     new_events =
//         kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
//     if (new_events == -1)
//       SocketUtils::exitWithPerror("kevent() error\n" +
//                                   std::string(strerror(errno)));

//     change_list.clear();  // clear change_list for new changes

//     for (int i = 0; i < new_events; ++i) {
//       curr_event = &event_list[i];

//       /* check error event return */
//       if (curr_event->flags & EV_ERROR) {
//         if (curr_event->ident == server_socket)
//           SocketUtils::exitWithPerror("server socket error");
//         else {
//           std::cerr << "client socket error" << std::endl;
//           SocketUtils::disconnectClient(curr_event->ident, clients);
//         }
//       } else if (curr_event->filter == EVFILT_READ) {
//         if (curr_event->ident == server_socket) {
//           /* accept new client */
//           int client_socket;
//           if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
//             SocketUtils::exitWithPerror("accept() error\n" +
//                                         std::string(strerror(errno)));
//           std::cout << "accept new client: " << client_socket << std::endl;
//           fcntl(client_socket, F_SETFL, O_NONBLOCK);

//           /* add event for client socket - add read && write event */
//           SocketUtils::changeEvents(change_list, client_socket, EVFILT_READ,
//                                     EV_ADD | EV_ENABLE, 0, 0, NULL);
//           SocketUtils::changeEvents(change_list, client_socket, EVFILT_WRITE,
//                                     EV_ADD | EV_ENABLE, 0, 0, NULL);
//           clients[client_socket] = "";
//         } else if (clients.find(curr_event->ident) != clients.end()) {
//           /* read data from client */
//           char buf[1024];
//           // frontcontroller로 fd 보내주고 응답 받기, 주석 처리한 부분에
//           응답을
//           // 받으면 client쪽으로 보내주기
//           int n = read(curr_event->ident, buf, sizeof(buf));
//           if (n <= 0) {
//             if (n < 0) std::cerr << "client read error!" << std::endl;
//             SocketUtils::disconnectClient(curr_event->ident, clients);
//           } else {
//             buf[n] = '\0';
//             clients[curr_event->ident] += "euijin";
//             // std::cout << "received data from " << curr_event->ident << ":
//             "
//             // << clients[curr_event->ident] << std::endl;
//           }
//         }
//       } else if (curr_event->filter == EVFILT_WRITE) {
//         /* send data to client */
//         std::map<int, std::string>::iterator it =
//             clients.find(curr_event->ident);
//         if (it != clients.end()) {
//           if (clients[curr_event->ident] != "") {
//             int n;
//             //
//             clients[curr_event->ident] += " plus!";
//             if ((n = write(curr_event->ident,
//                            clients[curr_event->ident].c_str(),
//                            clients[curr_event->ident].size()) == -1)) {
//               std::cerr << "client write error!" << std::endl;
//               SocketUtils::disconnectClient(curr_event->ident, clients);
//             } else
//               clients[curr_event->ident].clear();
//           }
//         }
//       }
//     }
//   }
// }
