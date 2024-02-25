
#ifndef READ_EVENT_BEAN__HPP
#define READ_EVENT_BEAN__HPP
#include "Bean.hpp"

class HttpHandler;
class Event;
class Bean;
class ReadEventBean : public Bean {
 private:
  void responseSaveEvent(std::string body, HttpHandler *httpHandler,
                         Event *event);
  void errorSaveEvent(HttpHandler *httpHandler, Event *event);

 public:
  ReadEventBean();
  ~ReadEventBean();
  int runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif