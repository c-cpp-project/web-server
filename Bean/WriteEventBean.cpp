#include"WriteEventBean.hpp"

void    WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    int                 writeFd;
    std::string         data;

    writeFd = httpHandler->getFd();
    data = httpHandler->getData();
    write(writeFd, data.c_str(), data.length());
    close(writeFd);
    delete httpHandler;
}