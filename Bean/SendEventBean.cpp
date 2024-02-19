#include"SendEventBean.hpp"

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

void    SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    ServerConfiguration *serverConfig;
    HttpResponse        *response;

    serverConfig = httpHandler->getServerConfiguration();
    response = &httpHandler->getHttpResponse();
    if (response == 0) // flush -> delete
        return ;
    response->flush();
    event->saveEvent(response->getSockfd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
    delete httpHandler;
    // delete response;
}