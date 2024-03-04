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
  if (httpHandler->getDataLength() - httpHandler->getBufferIdx() < WRITE_BUFFER)
    buf_size = httpHandler->getDataLength() - httpHandler->getBufferIdx();
  ret = write(writeFd, httpHandler->getBufferStartIdx() + httpHandler->getBufferIdx(), buf_size);
  std::cout << "WRITE RETURN: " << ret << ", GET_BUFFER_IDX: " << httpHandler->getBufferIdx() << "\n";
  httpHandler->setBufferIdx(httpHandler->getBufferIdx() + ret);
  std::cout << "WRITE MOVED IDX: " << httpHandler->getBufferIdx() << "\n";
  if (ret < 0)
  {
    std::cout << "ERROR\n";
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);  // EVFILT_WRITE
    close(writeFd);
    delete httpHandler;
    httpHandler = 0;
    return (ret);
  }
  else if (httpHandler->getBufferIdx() == httpHandler->getDataLength() || ret == 0) {
    std::cout << "END\n";
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);  // EVFILT_WRIT
    close(writeFd);
    delete httpHandler;
    httpHandler = 0;
  }
  return ret;
} 