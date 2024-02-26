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
  std::cout << serverConfig << " = SendEventBean::runBeanEvent\n";
  if (response->flush() >= 0)
  {
    event->saveEvent(response->getSockfd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
    event->saveEvent(response->getSockfd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                    new HttpHandler(response->getSockfd(), serverConfig));
    delete httpHandler;
    std::cout << "================== SEND DONE ============================\n";
    return (0);
  } // 실패할 경우는 재시도를 하기 위하여 아무것도 변경하지 않는다.
  return (-1);
}