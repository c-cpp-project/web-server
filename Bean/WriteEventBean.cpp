#include"WriteEventBean.hpp"

WriteEventBean::WriteEventBean() {}
WriteEventBean::~WriteEventBean() {}

void    WriteEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    int                 writeFd;
    std::string         data;

    writeFd = httpHandler->getFd();
    data = httpHandler->getData();
    write(writeFd, data.c_str(), data.length());
    event->saveEvent(writeFd, EVFILT_WRITE, EV_DELETE, 0, 0, 0); // EVFILT_WRITE
    close(writeFd);
    delete httpHandler;
}