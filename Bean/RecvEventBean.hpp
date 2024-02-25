#ifndef RECV_EVENT_BEAN__HPP
#define RECV_EVENT_BEAN__HPP
#include "Bean.hpp"
#define RECV_COMPLETED 0
#define RECV_ONGOING -1
#define RECV_ERROR 1

class Bean;
class HttpHandler;
class RecvEventBean : public Bean {
 private:
 public:
  RecvEventBean();
  ~RecvEventBean();
  int runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif