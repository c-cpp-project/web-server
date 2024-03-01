#include "WriteEventBean.hpp"

WriteEventBean::WriteEventBean() {}
WriteEventBean::~WriteEventBean() {}

#define WRITE_BUFFER 65500
int WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  int     		writeFd;
  size_t  		idx;
  size_t  		ret;
  int         buf_size;
  std::string data;

  writeFd = httpHandler->getFd();
  data = httpHandler->getData();
  buf_size = WRITE_BUFFER;
  if (data.length() - httpHandler->getBufferIdx() < WRITE_BUFFER)
    buf_size = data.length() - httpHandler->getBufferIdx();
  ret = write(writeFd, data.c_str() + httpHandler->getBufferIdx(), buf_size);
  std::cout << "WRITE RETURN: " << ret;
  httpHandler->setBufferIdx(httpHandler->getBufferIdx() + ret);
  if (ret < 0)
    return (ret);
  else if (httpHandler->getBufferIdx() == data.length()) {
    std::cout << "END\n";
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);  // EVFILT_WRITE
    close(writeFd);
    delete httpHandler;
  }
  return ret;
} 