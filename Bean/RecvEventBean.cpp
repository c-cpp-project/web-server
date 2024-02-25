#include "RecvEventBean.hpp"

#include "../HttpRequest/HttpRequestHandler.hpp"

RecvEventBean::RecvEventBean() {}

RecvEventBean::~RecvEventBean() {}

// 만약 HttpHandler *가 하나라면, eventPendingvec.push_back(eventPending);
// return eventPendingvec;

// event->saveEvent(tcp_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new
// HttpHandler(tcp_socket, *ServerConfig));
int RecvEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  HttpRequestHandler httpRequestHandler(httpHandler->getFd(),
                                        httpHandler->getServerConfiguration());
  std::cout << httpHandler->getFd() << ", "
            << httpHandler->getServerConfiguration()
            << " = RecvEventBean::runBeanEvent\n";
  int ret = -2;
  try {
    // return value 받아와야 함
    ret = httpRequestHandler.handle(event);
  } catch (const std::exception &e) {
    ret = 0;
  }
  std::cout << "[RET] " << ret << std::endl;
  event->saveEvent(httpHandler->getFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
  delete httpHandler;
  return ret;
}
