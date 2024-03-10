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

#include "../HttpRequest/HttpRequestHandler.hpp"
#include "StringUtils.hpp"

std::map<std::pair<std::string, int>, ServerConfiguration*>
    WebServer::serverConfigs;

WebServer& WebServer::getInstance(
    std::map<std::pair<std::string, int>, ServerConfiguration*> serverConfig,
    int option) {
  static WebServer instance(serverConfig, option);
  return instance;
}

WebServer::WebServer(
    std::map<std::pair<std::string, int>, ServerConfiguration*> serverConfigs,
    int option)
    : option(option) {
  WebServer::serverConfigs = serverConfigs;
}

void WebServer::segSignalHandler(int signo) {
  static_cast<void>(signo);
  std::cout << "[Error] Segmentation Fault is detected" << std::endl;
  std::cout << "[Error] your configuration is wrong" << std::endl;
  exit(1);
}

void WebServer::init() {
  std::map<std::pair<std::string, int>, ServerConfiguration*>::iterator it =
      WebServer::serverConfigs.begin();
  signal(SIGSEGV, WebServer::segSignalHandler);
  int port = 8080;
  if (eventHandler.initKqueue()) {
    std::cout << "kqueue() error" << std::endl;
    exit(1);
  }
  while (it != WebServer::serverConfigs.end()) {
    ServerConfiguration* serverConfig = it->second;
    int serversSocket = openPort(serverConfig);
    std::cout << serversSocket << " [SOCKET]\n";
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
  freeaddrinfo(info);
  if (serverSocket == -1)
    SocketUtils::exitWithPerror("[Error] socket() error\n" +
                                std::string(strerror(errno)));
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (ports[port] == 1) {
    return serverSocket;
  }
  ports[port] = 1;
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
  ChildProcess childprocee(MAX_CPU_TIME_USED);
  BeanFactory baneFactory;
  while (true) {
    newEventCount = eventHandler.create();
    std::cout << newEventCount << " = newEventCount\n";
    if (newEventCount == -1) {
      SocketUtils::exitWithPerror("[ERROR] kevent() error\n" +
                                  std::string(strerror(errno)));
    }
    eventHandler.clearChangedEventList();
    for (int i = 0; i < newEventCount; i++) {
      processEvent(eventHandler[i]);
    }
    ChildProcess::waitChildProcess();
    // clearClients();
  }
}

void WebServer::processEvent(struct kevent& currEvent) {
  if (currEvent.filter != EVFILT_TIMER && (currEvent.udata == NULL)) {
    return;
  }
  if (currEvent.flags & EV_ERROR) {
    std::cout << "[CAUSE]\n";
    std::cout << currEvent << "\n";
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    if (handler != NULL) {
      delete handler;
      handler = NULL;
    }
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
    std::cout << currEvent << std::endl;
    disconnectClient(currEvent.ident);
    // addCandidatesForDisconnection(currEvent.ident);
    std::cout << currEvent.ident << "[INFO] client disconnected" << std::endl;
  }
}

void WebServer::processReadEvent(struct kevent& currEvent) {
  // TODO: 고치기
  std::cout << "processReadEvent currEvent: " << currEvent << std::endl;
  if (hasServerFd(currEvent)) {
    acceptClient(currEvent.ident);
  } else if (isClient(currEvent.ident)) {
    std::cout << currEvent.ident << " = RECV currEvent.ident\n";
    std::cout << "[RECV] DONE " << (currEvent.flags & EV_EOF) << std::endl;
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    int ret = BeanFactory::runBeanByName("RECV", handler, &eventHandler);
    std::cout << "[INFO] RET" << ret << std::endl;
    if (ret == 1) {
      disconnectClient(currEvent.ident);
    }
  } else {
    std::cout << currEvent.ident << " = READ currEvent.ident\n";
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    BeanFactory::runBeanByName("READ", handler, &eventHandler);
  }
}

void WebServer::processWriteEvent(struct kevent& currEvent) {
  // BeanFactory beanFactory;
  std::cout << "processWriteEvent currEvent" << currEvent << std::endl;
  if (isClient(currEvent.ident)) {
    if (currEvent.flags & EV_EOF) {
      std::cout << "[CHECK SEND ERROR]" << std::endl;
      // addCandidatesForDisconnection(currEvent.ident);
      HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
      delete handler;
      disconnectClient(currEvent.ident);
    }
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    // ServerConfiguration* serverConfig = handler->getServerConfiguration();
    int ret = BeanFactory::runBeanByName("SEND", handler, &eventHandler);
    if (ret < 0) {
      disconnectClient(currEvent.ident);
    }
  } else {
    // CGI
    std::cout << "WRITE currEvent " << currEvent << std::endl;
    HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
    // ServerConfiguration* serverConfig = handler->getServerConfiguration();
    BeanFactory::runBeanByName("WRITE", handler, &eventHandler);
  }
}

void WebServer::processTimerEvent(struct kevent& currEvent) {
  std::cout << "WebServer::processTimerEvent START: " << currEvent.ident << "\n";
  eventHandler.saveEvent(currEvent.ident, EVFILT_TIMER, EV_DISABLE, 0, 0, 0);
  if (handlerMap.find(currEvent.ident) == handlerMap.end()) return;
  HttpHandler* handler = reinterpret_cast<HttpHandler*>(currEvent.udata);
  delete handler;
  disconnectClient(currEvent.ident);
  // addCandidatesForDisconnection(currEvent.ident);
}

int WebServer::acceptClient(int serverSocket) {
  std::cout << "acceptClient\n";
  struct _linger linger;
  linger.l_onoff = 1;
  linger.l_linger = 3;
  const int clientSocket = accept(serverSocket, NULL, NULL);
  std::pair<std::string, int> serverIdentifier =
      serverSocketPortMap[serverSocket];
  const int serverPort = serverIdentifier.second;
  setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, &linger, sizeof(_linger));
  if (clientSocket == -1) {
    std::cout << "[ERROR] accept() error" << std::endl;
    return -1;
  }
  fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
  ServerConfiguration* serverConfig =
      WebServer::serverConfigs[serverIdentifier];
  addClient(clientSocket, serverConfig, &eventHandler);
  eventHandler.registerEnabledReadEvent(clientSocket, handlerMap[clientSocket]);
  eventHandler.registerDisabledWriteEvent(clientSocket,
                                          handlerMap[clientSocket]);
  eventHandler.saveEvent(clientSocket, EVFILT_TIMER, EV_ADD | EV_ENABLE,
                         NOTE_SECONDS, 150, handlerMap[clientSocket]);
  return clientSocket;
}

bool WebServer::hasServerFd(struct kevent& currEvent) {
  return serverSocketPortMap.find(currEvent.ident) != serverSocketPortMap.end();
}

void WebServer::disconnectPort(struct kevent& currEvent) {
  close(currEvent.ident);
  WebServer::serverConfigs.erase(serverSocketPortMap[currEvent.ident]);
}

bool WebServer::isClient(int clientFd) {
  return handlerMap.find(clientFd) != handlerMap.end();
}

void WebServer::disconnectClient(int clientFd) {
  std::cout << "WebServer::disconnectClient CLOSE: " << clientFd << "\n";
  close(clientFd);
  handlerMap.erase(clientFd);
  HttpRequestHandler::removeAndDeleteChunkedRequest(clientFd);
  HttpRequestHandler::removeBuffer(clientFd);
}

void WebServer::clearClients() {
  std::set<int>::iterator it = candidatesForDisconnection.begin();
  for (; it != candidatesForDisconnection.end(); it++) {
    std::map<int, HttpHandler*>::iterator handlerIterator =
        handlerMap.find(*it);
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
  handlerMap[clientFd] = new HttpHandler(
      clientFd, serverConfig);  // TODO: 메모리 delete 확인해주기
  return clientFd;
};