#include "SendEventBean.hpp"

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

int SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  ServerConfiguration *serverConfig;
  HttpResponse *response;

  response = &httpHandler->getHttpResponse();
  serverConfig = response->getServerConfiguration();
  if (response == 0)  // flush -> delete
    return 0;
  response->flush();
  event->saveEvent(response->getSockfd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
  std::cout << serverConfig << " = SendEventBean::runBeanEvent\n";
  event->saveEvent(response->getSockfd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   new HttpHandler(response->getSockfd(), serverConfig));
  delete httpHandler;
  std::cout << "================== SEND DONE ============================\n";
  return 0;
}