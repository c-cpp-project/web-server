#include "WriteEventBean.hpp"

WriteEventBean::WriteEventBean() {}
WriteEventBean::~WriteEventBean() {}

#define WRITE_BUFFER 1024 * 1024
int WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  int     		writeFd;
  int      		ret;
  int         buf_size;

  writeFd = httpHandler->getFd();
  buf_size = WRITE_BUFFER;
  if (httpHandler->getData().length() - httpHandler->getBufferIdx() < WRITE_BUFFER)
    buf_size = httpHandler->getData().length() - httpHandler->getBufferIdx();
  ret = write(writeFd, httpHandler->getData().c_str() + httpHandler->getBufferIdx(), buf_size);
  std::cout << "WRITE RETURN: " << ret << ", GET_BUFFER_IDX: " << httpHandler->getBufferIdx() << "\n";
  httpHandler->setBufferIdx(httpHandler->getBufferIdx() + ret);
  std::cout << "WRITE MOVED IDX: " << httpHandler->getBufferIdx() << "\n";
  if (ret < 0)
  {
    std::cout << "ERROR\n";
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);  // EVFILT_WRITE
    close(writeFd);
    delete httpHandler;
    return (ret);
  }
  else if (httpHandler->getBufferIdx() == httpHandler->getData().length()) {
    std::cout << "END\n";
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);  // EVFILT_WRITE
    close(writeFd);
    delete httpHandler;
  }
  return ret;
} 