#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
#include <map>

#include "Event.hpp"
#include "ServerConfiguration.hpp"
#include "SocketUtils.hpp"

class WebServer {
 public:
  static const int LISTENCAPACITY = 1000;
  static WebServer& getInstance(
      std::map<int, ServerConfiguration> serverConfigs);
  void run();
  void execute();

 private:
  Event eventHandler;
  std::map<int, ServerConfiguration> serverConfigs;
  std::map<int, int> serverSocketPortMap;
  WebServer(const WebServer&);
  WebServer(std::map<int, ServerConfiguration> serverConfig);
  //   WebServer& operator=(const WebServer&);
  static void segSignalHandler(int signo);
  void init();
  void handleEvent();
  int openPort(ServerConfiguration& serverConfig);

  void processEvent(struct kevent& currEvent);
  void processErrorEvent(struct kevent& currEvent);
  bool isValidFd(struct kevent& currEvent);
  void disconnectPort(struct kevent& currEvent);
  //   bool isResponseToServer(struct kevent& currEvent);
  //   void serverDisconnect(struct kevent& currEvent);
  //   void timerEventProcess(struct kevent& currEvent);
  //   void writeEventProcess(struct kevent& currEvent);
  void processReadEvent(struct kevent& currEvent);
  int acceptClient(int serverSocket);
};

struct _linger {
  int l_onoff;
  int l_linger;
};

#endif
