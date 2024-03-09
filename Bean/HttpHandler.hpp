
#ifndef HTTP_HANDLER_HPP
# define HTTP_HANDLER_HPP

#include"../HttpRequest/HttpRequest.hpp"
#include"../ResponseHandler/HttpResponse.hpp"

class HttpRequest;
class HttpResponse;
class ServerConfiguration;
class HttpHandler
{
private:
	int                 fd;
	HttpRequest         request;
	HttpResponse        response;
	ServerConfiguration *serverConfig;
	std::string			data;
	long long			bodySize;
	long long			bufferIdx;
	bool				close;
	HttpHandler();
public:
	HttpHandler(int fd, HttpRequest req, HttpResponse res);
	HttpHandler(int fd, HttpRequest req, HttpResponse res, long long bodySize);
	HttpHandler(int fd, HttpRequest req, HttpResponse res, ServerConfiguration *serverConfig);
	HttpHandler(int fd, HttpRequest req, HttpResponse res,  std::string data, ServerConfiguration *serverConfig);
	HttpHandler(int fd, HttpResponse res, long long size);
	HttpHandler(int fd, std::string data, ServerConfiguration *serverConfig);
	HttpHandler(int fd, ServerConfiguration *serverConfig);
	~HttpHandler();
	HttpHandler&	operator=(const HttpHandler& ref);
	HttpHandler(const HttpHandler& ref);
	int				getFd(void);
	HttpRequest		&getHttpRequest(void);
	HttpResponse	&getHttpResponse(void);
	ServerConfiguration*	getServerConfiguration(void);
	std::string		getData(void);
	void    		setData(std::string data);
	int     		getDataLength(void);
	long long   	getBodySize(void);
	void    		setBufferIdx(long long bufferIdx);
	long long   	getBufferIdx(void);
	const char  	*getBufferStartIdx(void);
	void			setConnectionClose(bool value);
	bool			getConnectionClose(void);
};
// HttpResponse 대입 연산자를 만들어야 한다.
#endif