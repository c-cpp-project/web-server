
#ifndef WRITE_EVENT_BEAN__HPP
# define WRITE_EVENT_BEAN__HPP

#include"Bean.hpp"
class WriteEventBean : public Bean
{
private:
public:
    WriteEventBean();
    ~WriteEventBean();
    void    runBeanEvent(HttpHandler *httpHandler, Event *event);
};
#endif