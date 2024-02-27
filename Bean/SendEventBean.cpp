#include "SendEventBean.hpp"

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

#define BUF_SIZE 1024
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
  std::cout << socketfd << ", " << dump.length() << ": size => " << BUF_SIZE
            << "\n";
  try {
    if (dump.length() < BUF_SIZE) buf_size = dump.length();
    ret = send(socketfd, dump.c_str(), buf_size, 0);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    std::cout << "FUCK\n";
  }
  std::cout << ret << " = ret\n";
  if (ret < 0) {
    return -1;
  }
  if (ret == 0) {
    event->saveEvent(socketfd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
    event->saveEvent(socketfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                     new HttpHandler(socketfd, serverConfig));
    delete httpHandler;
    std::cout << "================== SEND DONE ============================\n";
    return (0);
  } else
    httpHandler->setData(dump.substr(ret));
  return (ret);
}