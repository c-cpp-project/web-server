#include"HttpResponse.hpp"

HttpResponse::HttpResponse()
{}

HttpResponse::HttpResponse(int sockfd) // default 지정
{
	this->sockfd = sockfd;
	this->send_timeout = "60"; // nginx send_timeout default
	this->max_size = 64 * K;
	this->status_code = "200";

	// header에 대한 기본적인 설정 완료하기
	putHeader("Keep-Alive", "timeout=60, max=999");
}

HttpResponse::HttpResponse(int sockfd, std::string send_timeout)
{
	this->sockfd = sockfd;
	this->send_timeout = send_timeout; // nginx send_timeout default
}

void    HttpResponse::putHeader(std::string key, std::string value)
{
	headers.insert(std::make_pair(key, value));
}

void	HttpResponse::redirect(HttpRequest &request, HttpResponse &response)
{
	response.getMaxBodySize();
	setStatusCode("302");
	ResponseStatusLine();
	putHeader("Location", HttpConfig::getRedirectPath(request.getPath()));
	putHeader("Content-Type", "text/html");
	putHeader("Content-Length", "0");
	sendBody("");
}

void	HttpResponse::forward(HttpRequest &request, HttpResponse &response) // controller에서 사용한다.
{
	std::string	uri;
	int			fd;
	std::string bodyLength;
	std::stringstream ss;
	std::string	body;

	uri = request.getPath();
	if (response.getStatusCode()[0] == '4' || response.getStatusCode()[0] == '5') // fail.page
		uri = "fail";
	fd = open(HttpConfig::pathResolver(uri).c_str(), O_RDONLY);
	if ((fd < 0 || request.getMethod() != "GET") && uri != "fail") // exception
		throw ErrorResponse("404", HttpConfig::getHttpStatusMsg("404"));
	body = readFile(fd);
	ss << body.length();
	bodyLength = ss.str();
	if (uri.compare(uri.length() - 4, 4, ".css") == 0)
		putHeader("Content-Type", "text/css");
	else
		putHeader("Content-Type", "text/html;charset=utf-8");
	putHeader("Content-Length", bodyLength);
	sendBody(body);
}

std::string	HttpResponse::readFile(int fd)
{
	int			ret;
	int			size;
	char		body[64 * K];

	size = 0;
	while ((ret = read(fd, &body[size], K)) > 0)
		size += ret;
	close(fd);
	return (body);
}

void    HttpResponse::sendBody(std::string body) // api 요청에 대한 응답
{
	ResponseStatusLine();
	processHeader();
	HttpResponseBody(body);
	flush();
}

void    HttpResponse::ResponseStatusLine()
{
	std::string	msg  = "";

	msg = "HTTP/1.1" + this->status_code + " " + HttpConfig::getHttpStatusMsg(this->status_code);
	this->buffer.push_back(msg);
}

void    HttpResponse::processHeader()
{
	std::string	msg  = "";
	std::map<std::string, std::string>::iterator it;
	
	for (it = this->headers.begin(); it != this->headers.end(); ++it) 
		msg += (it->first + ": " + it->second + "\r\n");
	msg += "\r\n";
	this->buffer.push_back(msg);
}

void	HttpResponse::HttpResponseBody(std::string body)
{
	if (body == "")
		return ;
	this->buffer.push_back(body);
}

void	HttpResponse::flush() // 마지막에 호출
{
	unsigned int	i;
	int				size;
	std::string		httpMsg;

	i = 0;
	size = 0;
	while (i < this->buffer.size())
	{
		size += this->buffer[i].length();
		i++;
	}
	if (size > this->max_size) // Payload Too Large
		throw ErrorResponse("413", HttpConfig::getHttpStatusMsg("413"));
	httpMsg = "";
	i = 0;
	while (i < this->buffer.size())
	{
		httpMsg += this->buffer[i];
		i++;
	}
	send(this->sockfd, httpMsg.c_str(), httpMsg.length(), 0);
}

void	HttpResponse::setStatusCode(std::string code)
{
	this->status_code = code;
}

std::string	HttpResponse::getStatusCode()
{
	return (this->status_code);
}

long long	HttpResponse::getMaxBodySize()
{
	return (this->max_size);
}

int		HttpResponse::getSockfd()
{
	return (this->sockfd);
}