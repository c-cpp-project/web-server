#ifndef EventManager_HPP
#define EventManager_HPP
#include <unistd.h>

#include "Event.hpp"
#include "FrontController.hpp"
#include "ServerConfiguration.hpp"

class EventManager {
 private:
  std::map<int, FrontController> controllers;
  /* data */
 public:
  EventManager(/* args*/);
  ~EventManager();
  int addNewClient(int serverFd, ServerConfiguration* serverConfig,
                   Event* event);
  void disconnectClient(int clientFd);
  FrontController& getFrontController(int clientFd);
  bool isClient(int clientFd);

  bool processReadEvent(struct kevent& currEvent);
  bool processWriteEvent(struct kevent& currEvent);
};

#endif