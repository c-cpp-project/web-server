
#ifndef HTTP_HANDLER_HPP
# define HTTP_HANDLER_HPP

#include"../HttpRequest/HttpRequest.hpp"
#include"../ResponseHandler/HttpResponse.hpp"
#include"../server/ServerConfiguration.hpp"
// #include"../server/Event.hpp"
// event 등록을 다른 class에서 수행합니다.
class HttpRequest;
class HttpResponse;
class ServerConfiguration;
class HttpHandler
{
private:
	int                 fd;
	HttpRequest         *request;
	HttpResponse        *response;
	ServerConfiguration *serverConfig;
	std::string			data;
	// Event               event;
	HttpHandler();
public:
	HttpHandler(int fd, HttpRequest *req, HttpResponse *res);
	HttpHandler(int fd, HttpResponse *res);
	HttpHandler(int fd, std::string data, ServerConfiguration *serverConfig);
	HttpHandler(int fd, ServerConfiguration *serverConfig);
	~HttpHandler();
	HttpHandler&	operator=(const HttpHandler& ref);
	int				getFd(void);
	HttpRequest		&getHttpRequest(void);
	HttpResponse	&getHttpResponse(void);
	ServerConfiguration*	getServerConfiguration(void);
	std::string		getData(void);
};
// HttpResponse 대입 연산자를 만들어야 한다.
#endif