
#ifndef SEND_EVENT_BEAN__HPP
# define SEND_EVENT_BEAN__HPP

#include"Bean.hpp"
class SendEventBean : public Bean
{
private:
public:
    SendEventBean();
    ~SendEventBean();
    void    runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif