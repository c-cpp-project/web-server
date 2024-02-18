
#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP
#include"../HttpRequest/HttpRequest.hpp"
#include"../ResponseConfig.hpp"
#include"sys/fcntl.h"
#include"../server/Event.hpp"
#include"../server/ServerConfiguration.hpp"
#include<map>
#include<sys/socket.h>
#include<unistd.h>
#include <sstream>
#include<vector>
#include<string>
#include <cstdlib>

# define K 1000

class HttpRequest;
class ServerConfiguration;
class HttpResponse
{
private:
	int         sockfd;
	std::map<std::string, std::string> headers;
	std::vector<std::string>    buffer;
	unsigned long	max_size;
	std::string	send_timeout; // WAS
	std::string	status_code;
	std::string	responseBody;
	bool		authenticated;
	bool		overlaped;
	ServerConfiguration *serverConfig;
	Event				*event;
public:
	HttpResponse();
	HttpResponse(int sockfd);
	HttpResponse(int sockfd, ServerConfiguration *serverConfig, Event *event); // default 지정
	HttpResponse(int sockfd, std::string send_timeout, ServerConfiguration *serverConfig, Event *event);
	HttpResponse(int sockfd, int max_size, ServerConfiguration *serverConfig, Event *event); // transfer-tokenizer

	HttpResponse& operator=(const HttpResponse& ref);

	std::string	findValue(std::string key);
	void    putHeader(std::string key, std::string value);
	void	removeHeader(std::string key);
	void	HttpResponseBody(std::string body);
	std::string	readFile(int fd);

	void	redirect(std::string redirectUri);
	void    forward(HttpRequest &Httprequest);
	void    sendBody(std::string body);
	void    ResponseStatusLine();
	void    processHeader();
	
	void	tokenizerFlush(std::string body);
	void    flush(); // 마지막에 호출
	void	setStatusCode(std::string code);
	std::string	getStatusCode();

	long long	getMaxBodySize();
	int		getSockfd();
	
	void		putContentType(std::string filename);
	std::string	readRangeQuery(std::string range, std::string body);
	ServerConfiguration	*getServerConfiguration(void);
	Event	*getEvent(void);
};
#endif