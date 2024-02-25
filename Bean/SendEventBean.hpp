
#ifndef SEND_EVENT_BEAN__HPP
# define SEND_EVENT_BEAN__HPP

#include"Bean.hpp"

class HttpHandler;
class Bean;
class SendEventBean : public Bean
{
private:
public:
    SendEventBean();
    ~SendEventBean();
    int    runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif