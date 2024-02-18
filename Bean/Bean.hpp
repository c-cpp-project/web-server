#ifndef BEAN_HPP
# define BEAN_HPP
#include"../server/Event.hpp"
#include"HttpHandler.hpp"
#include<vector>

class HttpHandler;
class Bean
{
public:
    Bean();
    virtual ~Bean();
    virtual void    runBeanEvent(HttpHandler *httpHandler, Event *event) = 0;
};
#endif