#include"RecvEventBean.hpp"
#include "../HttpRequest/HttpRequestHandler.hpp"

RecvEventBean::RecvEventBean() {}

RecvEventBean::~RecvEventBean() {}

// 만약 HttpHandler *가 하나라면, eventPendingvec.push_back(eventPending);
// return eventPendingvec;

// event->saveEvent(tcp_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(tcp_socket, *ServerConfig));
void    RecvEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    HttpRequestHandler httpRequestHandler(httpHandler->getFd(), httpHandler->getServerConfiguration());
    httpRequestHandler.handle(event);
    event->saveEvent(httpHandler->getFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
    delete httpHandler;
}