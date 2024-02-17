
#ifndef READ_EVENT_BEAN__HPP
# define READ_EVENT_BEAN__HPP

#include"Bean.hpp"
class ReadEventBean : public Bean
{
private:
public:
    ReadEventBean();
    ~ReadEventBean();
    void    runBeanEvent(HttpHandler *httpHandler, Event *event);
    void	response200(std::string body, HttpHandler *httpHandler, Event *event);
};
#endif