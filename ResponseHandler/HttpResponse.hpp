// 단순하게 바디 크기는 32K, WAS 대기 시간은 60s (cgi), keep-alive 75s -> Httprequest

#include"../HttpRequest/HttpRequest.hpp"
#include"../HttpConfig.hpp"
#include"sys/fcntl.h"
#include"../controllers/ControllerMapping.hpp"
#include<map>
#include<sys/socket.h>
#include<unistd.h>
#include <sstream>
#include<vector>
#include<string>

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP
# define K 1000

class HttpConfig;
class HttpRequest;
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
public:
	HttpResponse();
	HttpResponse(int sockfd); // default 지정
	HttpResponse(int sockfd, std::string send_timeout);
	HttpResponse(int sockfd, int max_size); // transfer-tokenizer

	std::string	findValue(std::string key);
	void    putHeader(std::string key, std::string value);
	void	removeHeader(std::string key);
	void	HttpResponseBody(std::string body);
	std::string	readFile(int fd);

	void	redirect(HttpRequest &Httprequest, HttpResponse &Httpresponse);
	void    forward(HttpRequest &Httprequest, HttpResponse &Httpresponse);
	void    sendBody(std::string body);
	void    ResponseStatusLine();
	void    processHeader();
	
	void	tokenizerFlush(std::string body);
	void    flush(); // 마지막에 호출
	void	setStatusCode(std::string code);
	std::string	getStatusCode();

	long long	getMaxBodySize();
	int		getSockfd();
};
#endif