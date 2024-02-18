#ifndef RECV_EVENT_BEAN__HPP
# define RECV_EVENT_BEAN__HPP
#include"Bean.hpp"

class Bean;
class HttpHandler;
class RecvEventBean : public Bean
{
private:
public:
    RecvEventBean();
    ~RecvEventBean();
    void    runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif