#include"RecvEventBean.hpp"
#include "../HttpRequest/HttpRequestHandler.hpp"

RecvEventBean::RecvEventBean() {}

RecvEventBean::~RecvEventBean() {}

// 만약 HttpHandler *가 하나라면, eventPendingvec.push_back(eventPending);
// return eventPendingvec;
void    RecvEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    HttpRequestHandler httpRequestHandler(httpHandler->getFd(), httpHandler->getServerConfiguration());
    httpRequestHandler.handle(event);
    delete httpHandler;
}