#include "WriteEventBean.hpp"

WriteEventBean::WriteEventBean() {}
WriteEventBean::~WriteEventBean() {}

#define BUF_SIZE 1024 * 1024

int WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  int writeFd;
  size_t idx;
  size_t ret;
  std::string data;

  writeFd = httpHandler->getFd();
  data = httpHandler->getData();
  int buf_size = BUF_SIZE;
  if (data.length() < buf_size) {
    buf_size = BUF_SIZE;
  }
  ret = write(writeFd, data.c_str(), buf_size);
  std::cout << " WriteEventBean::runBeanEvent: " << writeFd << "["
            << data.length() << ", " << ret << "]\n";
  if (ret <= 0) {
    close(writeFd);
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DELETE, 0, 0,
                     0);  // EVFILT_WRITE
    delete httpHandler;
    return (ret);
  } else
    httpHandler->setData(data.substr(ret));
  return ret;
}