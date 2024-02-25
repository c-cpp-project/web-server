
#ifndef WRITE_EVENT_BEAN__HPP
#define WRITE_EVENT_BEAN__HPP

#include "Bean.hpp"
class HttpHandler;
class Bean;
class WriteEventBean : public Bean {
 private:
 public:
  WriteEventBean();
  ~WriteEventBean();
  int runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif