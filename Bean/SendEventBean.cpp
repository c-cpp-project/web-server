#include "SendEventBean.hpp"

#include <sys/socket.h>

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

#define BUF_SIZE 65500
int SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  ServerConfiguration *serverConfig;
  std::string dump;
  int socketfd;
  int ret;
  int buf_size;

  serverConfig = httpHandler->getServerConfiguration();
  socketfd = httpHandler->getFd();
  dump = httpHandler->getData();
  std::cout << serverConfig << " = SendEventBean::runBeanEvent\n";
  buf_size = BUF_SIZE;
  if (dump.length() - httpHandler->getBufferIdx() < BUF_SIZE)
    buf_size = dump.length() - httpHandler->getBufferIdx();
  ret = write(socketfd, dump.c_str() + httpHandler->getBufferIdx(), buf_size);
  std::cout << "WRITE RETURN: " << ret << "\n";
  std::cout << "[" << std::string(dump.c_str() + httpHandler->getBufferIdx(), ret) << "]\n";
  httpHandler->setBufferIdx(httpHandler->getBufferIdx() + ret);
  if (ret < 0)
    return (ret);
  else if (httpHandler->getBufferIdx() == dump.length()) {
    event->saveEvent(socketfd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
    event->saveEvent(socketfd, EVFILT_READ, EV_ENABLE, 0, 0,
                     new HttpHandler(socketfd, serverConfig));
    delete httpHandler;
    std::cout << "================== SEND DONE ============================\n";
    return (0);
  }
  return (ret);
}