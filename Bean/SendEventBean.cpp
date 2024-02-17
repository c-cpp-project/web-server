#include"SendEventBean.hpp"

void    SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    ServerConfiguration *serverConfig;
    HttpResponse        *response;

    serverConfig = httpHandler->getServerConfiguration();
    response = &httpHandler->getHttpResponse();
    if (response == 0) // flush -> delete
        return ;
    response->flush();
    delete httpHandler;
    delete response;
}