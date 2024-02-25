#ifndef BEAN_HPP
#define BEAN_HPP
#include <vector>

#include "../server/Event.hpp"
#include "HttpHandler.hpp"

class HttpHandler;
class Bean {
 public:
  Bean();
  virtual ~Bean();
  virtual int runBeanEvent(HttpHandler *httpHandler, Event *event) = 0;
};
#endif