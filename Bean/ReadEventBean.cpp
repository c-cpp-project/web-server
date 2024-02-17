#include"ReadEventBean.hpp"

void    ReadEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event)
{
    HttpResponse        *response;
    std::string         body;
    int                 readFd;

    readFd = httpHandler->getFd();
    response = &httpHandler->getHttpResponse();
    if (response == 0) // flush -> delete.
        return ;
    body = response->readFile(readFd);
    if (body == "")
        throw "500";
    response200(body, httpHandler, event);
    delete httpHandler;
}

void	    ReadEventBean::response200(std::string body, HttpHandler *httpHandler, Event *event)
{
	std::stringstream   ss;
	std::string 	    bodyLength;
    ServerConfiguration *serverConfig;
    HttpResponse        &response = httpHandler->getHttpResponse();
    HttpRequest         &request = httpHandler->getHttpRequest();

	response.putHeader("Server", httpHandler->getServerConfiguration()->getServerName());
	response.putHeader("Date", ResponseConfig::getCurrentDate());
	response.putHeader("Content-Type", "text/html;charset=utf-8");
    if (request.getParameter("Range") != "" && request.getMethod() == "GET")
		body = response.readRangeQuery(request.getParameter("Range"), body);
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
    response.sendBody(body); // this->buffer에 string으로 모두 담긴다.
    BeanFactory::registerEvent("SEND", new HttpHandler(response.getSockfd(), &response), event);
    // Event 등록; flush()
}