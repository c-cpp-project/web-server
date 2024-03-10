#include "RecvEventBean.hpp"

#include "../HttpRequest/HttpRequestHandler.hpp"

RecvEventBean::RecvEventBean() {}

RecvEventBean::~RecvEventBean() {}

// 만약 HttpHandler *가 하나라면, eventPendingvec.push_back(eventPending);
// return eventPendingvec;

// event->saveEvent(tcp_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new
// HttpHandler(tcp_socket, *ServerConfig));
// 100sec
int RecvEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  HttpRequestHandler httpRequestHandler(httpHandler->getFd(),
                                        httpHandler->getServerConfiguration());
  std::cout << httpHandler->getFd() << ", "
            << httpHandler->getServerConfiguration()
            << " = RecvEventBean::runBeanEvent\n";
  int ret = 1; // 버퍼에 읽어온 요청이 불완전할 때 -1, 완전할 때 0, 소켓을 닫아야 할 때 1
  try {
    // return value 받아와야 함
    ret = httpRequestHandler.handle(event);
  } catch (SocketCloseException400 &e) {
    HttpRequestHandler::removeAndDeleteChunkedRequest(httpHandler->getFd());
    HttpRequestHandler::removeBuffer(httpHandler->getFd());
    ret = 0;
  } catch (SocketCloseException413 &e) {
    HttpRequestHandler::removeAndDeleteChunkedRequest(httpHandler->getFd());
    HttpRequestHandler::removeBuffer(httpHandler->getFd());
    ret = 0;
  }
  std::cout << "[RET] " << ret << std::endl;
  if (ret >= 0) {
    event->saveEvent(httpHandler->getFd(), EVFILT_READ, EV_DELETE, 0, 0, 0);
    event->saveEvent(httpHandler->getFd(), EVFILT_TIMER, EV_DELETE, 0, 0, 0);
    delete httpHandler;
    httpHandler = 0;
  }
  return ret;
}

// tcp socket 연결:100sec -> recv: timer 해제 -> ... -> send: timer 5sec (next
// recv 기다림)