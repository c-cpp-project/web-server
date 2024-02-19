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
    std::map<int, ServerConfiguration*> serverConfig) {
  static WebServer instance(serverConfig);
  return instance;
}

WebServer::WebServer(std::map<int, ServerConfiguration*> serverConfigs) {
  this->serverConfigs = serverConfigs;
}

void WebServer::segSignalHandler(int signo) {
  static_cast<void>(signo);
  std::cout << "[Error] Segmentation Fault is detected" << std::endl;
  std::cout << "[Error] your configuration is wrong" << std::endl;
  exit(1);
}

void WebServer::init() {
  std::map<int, ServerConfiguration*>::iterator it = serverConfigs.begin();
  signal(SIGSEGV, WebServer::segSignalHandler);
  int port = 8080;
  if (eventHandler.initKqueue()) {
    std::cout << "kqueue() error" << std::endl;
    exit(1);
  }
  while (it != serverConfigs.end()) {
    ServerConfiguration* serverConfig = it->second;
    int serversSocket = openPort(serverConfig);
    fcntl(serversSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    serverSocketPortMap[serversSocket] = it->first;
    eventHandler.registerServerEvent(serversSocket, serverConfig);
    it++;
  }
}

int WebServer::openPort(ServerConfiguration* serverConfig) {
  struct addrinfo* info;
  struct addrinfo hint;
  struct sockaddr_in socketaddr;
  int opt = 1;
  int port = serverConfig->getPort();
  std::string serverName = serverConfig->getServerName();

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
    SocketUtils::exitWithPerror("[Error] bind() error\n" +
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
      SocketUtils::exitWithPerror("[ERROR] kevent() error\n" +
                                  std::string(strerror(errno)));
    }
    eventHandler.clearChangedEventList();
    for (int i = 0; i < newEventCount; i++) {
      processEvent(eventHandler[i]);
    }
    clearClients();
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
      processWriteEvent(currEvent);
      break;
    case EVFILT_TIMER:
      processTimerEvent(currEvent);
      break;
  }
}

void WebServer::processErrorEvent(struct kevent& currEvent) {
  if (hasServerFd(currEvent)) {
    disconnectPort(currEvent);
    std::cout << currEvent.ident << "[INFO] server disconnected" << std::endl;
  } else {
    addCandidatesForDisconnection(currEvent.ident);
    std::cout << currEvent.ident << "[INFO] client disconnected" << std::endl;
  }
}

void WebServer::processReadEvent(struct kevent& currEvent) {
  // TODO: 고치기
  std::cout << "currEvent: " << currEvent << std::endl;
  if (hasServerFd(currEvent)) {
    acceptClient(currEvent.ident);
  } else if (isClient(currEvent.ident)) {
    std::cout << currEvent.ident << " = RECV currEvent.ident\n";
    if (currEvent.flags & EV_EOF) {
      addCandidatesForDisconnection(currEvent.ident);
    }
    BeanFactory beanFactory;
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    beanFactory.runBeanByName("RECV", handler, &eventHandler);
  } else {
    std::cout << currEvent.ident << " = READ currEvent.ident\n";
    BeanFactory beanFactory;
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    beanFactory.runBeanByName("READ", handler, &eventHandler);
  }
}

void WebServer::processWriteEvent(struct kevent& currEvent) {
  BeanFactory beanFactory;
  if (isClient(currEvent.ident)) {
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    beanFactory.runBeanByName("SEND", handler, &eventHandler);
  } else {
    // CGI
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    beanFactory.runBeanByName("WRITE", handler, &eventHandler);
  }
}

void WebServer::processTimerEvent(struct kevent& currEvent) {
  addCandidatesForDisconnection(currEvent.ident);
}

int WebServer::acceptClient(int serverSocket) {
  struct _linger linger;
  linger.l_onoff = 1;
  linger.l_linger = 0;
  const int clientSocket = accept(serverSocket, NULL, NULL);
  const int serverPort = serverSocketPortMap[serverSocket];
  setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, &linger, sizeof(_linger));
  if (clientSocket == -1) {
    std::cout << "[ERROR] accept() error" << std::endl;
    return -1;
  }
  fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
  ServerConfiguration* serverConfig = serverConfigs[serverPort];
  addClient(clientSocket, serverConfig, &eventHandler);
  eventHandler.registerEnabledReadEvent(
      clientSocket, new HttpHandler(clientSocket, serverConfigs[serverPort]));
  return clientSocket;
}

bool WebServer::hasServerFd(struct kevent& currEvent) {
  return serverSocketPortMap.find(currEvent.ident) != serverSocketPortMap.end();
}

void WebServer::disconnectPort(struct kevent& currEvent) {
  close(currEvent.ident);
  serverConfigs.erase(serverSocketPortMap[currEvent.ident]);
}

bool WebServer::isClient(int clientFd) {
  return handlerMap.find(clientFd) != handlerMap.end();
}

void WebServer::disconnectClient(int clientFd) {
  close(clientFd);
  handlerMap.erase(clientFd);
}

void WebServer::clearClients() {
  std::set<int>::iterator it = candidatesForDisconnection.begin();
  for (; it != candidatesForDisconnection.end(); it++) {
    std::map<int, Handler*>::iterator handlerIterator = handlerMap.find(*it);
    // handlerIterator->second
    // TODO: close 로직 세우기 협의 필요
    disconnectClient(*it);
  }
  candidatesForDisconnection.clear();
}

void WebServer::addCandidatesForDisconnection(int clientFd) {
  candidatesForDisconnection.insert(clientFd);
}

int WebServer::addClient(int clientFd, ServerConfiguration* serverConfig,
                         Event* eventHandler) {
  handlerMap[clientFd] = new Handler(
      clientFd, serverConfig, eventHandler);  // TODO: 메모리 delete 확인해주기
  return clientFd;
};