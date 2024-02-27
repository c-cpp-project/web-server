#include "WriteEventBean.hpp"

WriteEventBean::WriteEventBean() {}
WriteEventBean::~WriteEventBean() {}

int WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  int     writeFd;
  size_t  idx;
  size_t  ret;
  std::string data;

  writeFd = httpHandler->getFd();
  data = httpHandler->getData();
  ret = write(writeFd, data.c_str(), data.length());
  if (ret <= 0)
  {
    std::cout << " WriteEventBean::runBeanEvent: [" << data.length() << ", " << ret << "]\n";
    close(writeFd);
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);  // EVFILT_WRITE
    delete httpHandler;
    return (ret);
  }
  else
    httpHandler->setData(data.substr(ret));
  return ret;
} 