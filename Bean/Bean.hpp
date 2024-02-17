
#ifndef BEAN_HPP
# define BEAN_HPP
#include"../server/Event.hpp"
#include"HttpHandler.hpp"
#include<vector>
class Bean
{
public:
    Bean();
    ~Bean();
    virtual void    runBeanEvent(HttpHandler *httpHandler, Event *event) = 0;
};
#endif