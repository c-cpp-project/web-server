#ifndef EventManager_HPP
#define EventManager_HPP
#include <unistd.h>

#include "../HttpRequest/HttpRequestHandler.hpp"
#include "Event.hpp"
#include "ServerConfiguration.hpp"

class EventManager {
 private:
  std::map<int, HttpRequestHandler> requestHandlers;
  /* data */
 public:
  EventManager(/* args*/);
  ~EventManager();
  int addNewClient(int serverFd, ServerConfiguration* serverConfig,
                   Event* event);
  void disconnectClient(int clientFd);
  HttpRequestHandler& getRequestHandler(int clientFd);
  bool isClient(int clientFd);

  bool processReadEvent(struct kevent& currEvent);
  bool processWriteEvent(struct kevent& currEvent);
};

#endif