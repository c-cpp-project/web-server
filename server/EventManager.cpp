#include "EventManager.hpp"

int EventManager::addNewClient(int clientFd,
                               ServerConfiguration* serverConfiguration,
                               Event* event) {
  // FrontController 관련 Map만들 듯
  // 요청 당 하나의 FrontController가 만들어진다고 가정
  HttpRequestHandler handler =
      HttpRequestHandler(clientFd, serverConfiguration);
  requestHandlers[clientFd] = handler;
  // setter든 생성자는 로직 설정
  // controllers[client_fd].setServerConfiguration(server);
  // controllers[client_fd].setEvents(events);
  return clientFd;
};

void EventManager::disconnectClient(int clientFd) {
  close(clientFd);
  requestHandlers.erase(clientFd);
}

bool EventManager::isClient(int clientFd) {
  return (requestHandlers.find(clientFd) != requestHandlers.end());
}