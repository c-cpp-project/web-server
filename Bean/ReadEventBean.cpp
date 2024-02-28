#include "ReadEventBean.hpp"

ReadEventBean::ReadEventBean() {}
ReadEventBean::~ReadEventBean() {}

#define READ_BUF_SIZE 1024
int ReadEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
	int     			readFd;
	int     			ret;
	char		    	binaryData[READ_BUF_SIZE + 1];
	std::string 	body;

	std::cout << "ReadEventBean::runBeanEvent\n";
	readFd = httpHandler->getFd();
	ret = read(readFd, binaryData, READ_BUF_SIZE);
	body = httpHandler->getData() + std::string(binaryData, binaryData + ret);
	httpHandler->setData(body);
	if (ret == READ_BUF_SIZE)
		return (ret);
	else if (ret < 0)
		errorSaveEvent(httpHandler, event);
	else
		responseSaveEvent(body, httpHandler, event);
	delete httpHandler;
	return 0;
}

void ReadEventBean::errorSaveEvent(HttpHandler *httpHandler, Event *event) {
	HttpResponse &response = httpHandler->getHttpResponse();
	HttpRequest empty;

	std::cout << "ReadEventBean::errorSaveEvent\n";
	response.setStatusCode("500");
	response.forward(empty);
}

void ReadEventBean::responseSaveEvent(std::string body,
																			HttpHandler *httpHandler, Event *event) {
	std::stringstream ss;
	std::string bodyLength;
	ServerConfiguration *serverConfig;
	HttpResponse &response = httpHandler->getHttpResponse();
	HttpRequest &request = httpHandler->getHttpRequest();
	std::string data;

	serverConfig = response.getServerConfiguration();
	response.putHeader("Server", serverConfig->getServerName());
	response.putHeader("Date", ResponseConfig::getCurrentDate());
	if (request.getParameter("Range") != "" && request.getMethod() == "GET")
		body = response.readRangeQuery(request.getParameter("Range"), body);
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
	// response.putHeader("Content-Type", "image/png");
	response.sendBody(body);  // this->buffer에 string으로 모두 담긴다.
	std::cout << "ReadEventBean::response -> saveEvent\n";
	std::cout << bodyLength << " = Content-Length\n";

	data = response.getByteDump();
	event->saveEvent(response.getSockfd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(response.getSockfd(), data, serverConfig));  // EVFILT_READ, EVFILT_WRITE
}