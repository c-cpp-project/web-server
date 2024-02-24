#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
#include <map>

#include "../Bean/BeanFactory.hpp"
#include "ServerConfiguration.hpp"
#include "SocketUtils.hpp"
#include "mock/Handler.hpp"

class WebServer {
 public:
  static const int LISTENCAPACITY = 1000;
  static WebServer& getInstance(
      std::map<int, ServerConfiguration*> serverConfigs);
  void execute();

 private:
  Event eventHandler;
  std::map<int, ServerConfiguration*> serverConfigs;
  std::map<int, int> serverSocketPortMap;
  std::map<int, HttpHandler*>
      handlerMap;  // TODO: 요청 관련해서 이벤트, fd 관리 일괄적으로
  std::set<int> candidatesForDisconnection;
  WebServer(const WebServer&);
  WebServer(std::map<int, ServerConfiguration*> serverConfig);
  //   WebServer& operator=(const WebServer&);
  static void segSignalHandler(int signo);
  void init();
  void handleEvent();
  int openPort(ServerConfiguration* serverConfig);

  void processEvent(struct kevent& currEvent);
  void processErrorEvent(struct kevent& currEvent);
  bool hasServerFd(struct kevent& currEvent);
  void disconnectPort(struct kevent& currEvent);
  //   bool isResponseToServer(struct kevent& currEvent);
  //   void serverDisconnect(struct kevent& currEvent);
  //   void timerEventProcess(struct kevent& currEvent);
  //   void writeEventProcess(struct kevent& currEvent);
  void processReadEvent(struct kevent& currEvent);
  void processWriteEvent(struct kevent& currEvent);
  void processTimerEvent(struct kevent& currEvent);
  int acceptClient(int serverSocket);

  void disconnectClient(int clientFd);
  void clearClients();
  void addCandidatesForDisconnection(int clientFd);
  bool isClient(int clientFd);
  int addClient(int clientFd, ServerConfiguration* serverConfig,
                Event* eventHandler);
};

struct _linger {
  int l_onoff;
  int l_linger;
};

#endif
