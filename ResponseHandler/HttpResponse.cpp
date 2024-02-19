#include"HttpResponse.hpp"
#include"../Bean/HttpHandler.hpp"

HttpResponse::HttpResponse()
{}

HttpResponse::HttpResponse(int sockfd)
{
	this->sockfd = sockfd;
}

HttpResponse::HttpResponse(int sockfd, ServerConfiguration *serverConfig, Event *event) // default 지정
{
	this->sockfd = sockfd;
	this->send_timeout = "60"; // nginx send_timeout default
	this->max_size = 64 * K;
	this->status_code = "200";
	this->responseBody = "";
	this->overlaped = false;
	this->serverConfig = serverConfig;
	this->event = event;

	// header에 대한 기본적인 설정 완료하기
	putHeader("Keep-Alive", "timeout=60, max=999");
}

HttpResponse::HttpResponse(int sockfd, int max_size, ServerConfiguration *serverConfig, Event *event) // tokenizer
{
	this->sockfd = sockfd;
	this->send_timeout = "60"; // nginx send_timeout default
	this->max_size = max_size;
	this->status_code = "200";
	this->serverConfig = serverConfig;
	this->event = event;

	// header에 대한 기본적인 설정 완료하기
	putHeader("Keep-Alive", "timeout=60, max=999");
}

HttpResponse::HttpResponse(int sockfd, std::string send_timeout, ServerConfiguration *serverConfig, Event *event)
{
	this->sockfd = sockfd;
	this->send_timeout = send_timeout; // nginx send_timeout default
	this->serverConfig = serverConfig;
	this->event = event;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& ref)
{
	if (this == &ref)
		return (*this);
	sockfd = ref.sockfd;
	headers = ref.headers;
	buffer = ref.buffer;
	max_size = ref.max_size;
	send_timeout = ref.send_timeout;
	status_code = ref.status_code;
	responseBody = ref.responseBody;
	authenticated = ref.authenticated;
	overlaped = ref.overlaped;
	serverConfig = ref.serverConfig;
	event = ref.event;
	return (*this);
}

void    HttpResponse::putHeader(std::string key, std::string value)
{
	headers.insert(std::make_pair(key, value));
}

void	HttpResponse::removeHeader(std::string key)
{
	headers.erase(key);
	std::cout << "[remove header: " << key << "]\n";
}

void	HttpResponse::redirect(std::string redirectUri)
{
	setStatusCode("302");
	putHeader("Content-Type", "text/html");
	putHeader("Location", redirectUri);
	putHeader("Content-Length", "0");
	sendBody("");
	// BeanFactory::registerEvent("SEND", new HttpHandler(getSockfd(), *this), event);H
	event->saveEvent(getSockfd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(getSockfd(), *this)); // EVFILT_READ, EVFILT_WRITE
}

void	HttpResponse::forward(HttpRequest &request) // controller에서 사용한다.
{
	int			fd;
	std::string	uri;

	uri = request.getPath();
	if (getStatusCode()[0] == '4' || getStatusCode()[0] == '5') // fail.page
		uri = serverConfig->getErrorpageResourcePath(std::atoi(getStatusCode().c_str()));
	fd = open(uri.c_str(), O_RDONLY);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	if ((fd < 0 || request.getMethod() != "GET") && \
	serverConfig->getErrorpageResourcePath(std::atoi(getStatusCode().c_str())) != "") 
	{
		std::cout << fd << ", " << request.getMethod() << ", " << uri <<"\n";
		throw "404";
	}
	// BeanFactory::registerEvent("READ", new HttpHandler(fd, request, *this), event);
	event->saveEvent(getSockfd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(getSockfd(), *this)); // EVFILT_READ, EVFILT_WRITE
}

std::string	HttpResponse::readFile(int fd)
{
	int			ret;
	int			size;
	char		body[64 * K];

	size = 0;
	while ((ret = read(fd, &body[size], K)) > 0)
		size += ret;
	body[size] = 0;
	close(fd);
	if (ret < 0)
		return ("");
	return (body);
}

void	HttpResponse::tokenizerFlush(std::string body)
{
	std::stringstream 	ss;
	std::string			length;

	flush();
	ss << std::hex << this->max_size;
    length = ss.str();
	while (body.size() > this->max_size)
	{
		this->buffer.push_back(length + "\r\n");
		this->buffer.push_back(body.substr(0, this->max_size) + "\r\n");
		flush();
		body = body.substr(this->max_size);
	}
	ss.str("");
	ss << std::hex << body.size();
	length = ss.str();
	this->buffer.push_back(length + "\r\n");
	this->buffer.push_back(body + "\r\n");
	flush();
	this->buffer.push_back("0\r\n\r\n");
	flush();
}

void    HttpResponse::sendBody(std::string body) // api 요청에 대한 응답
{
	if (overlaped == false)
		overlaped = true;
	else
		return ;
	ResponseStatusLine();
	processHeader();
	HttpResponseBody(body);
	unsigned long	clientBodySize = 0;
	int				i;

	i = 0;
	while (i < static_cast<int>(this->buffer.size()))
	{
		clientBodySize += this->buffer[i].length();
		if (clientBodySize > this->max_size)
			throw "413";
		i++;
	}
}

void    HttpResponse::ResponseStatusLine()
{
	std::string	msg  = "";

	msg += "HTTP/1.1 ";
	msg += this->status_code + " ";
	msg += ResponseConfig::getHttpStatusMsg(this->status_code);
	msg += "\r\n";
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

	httpMsg = "";
	i = 0;
	size = 0;
	while (i < this->buffer.size())
	{
		httpMsg += this->buffer[i];
		size += this->buffer[i].length();
		i++;
	}
	std::cout << "==========================================================\n";
	std::cout << httpMsg << "\n";
	// fcntl(this->sockfd, F_SETFD, O_NONBLOCK); -> 어차피 O_NONBLOCK
	send(this->sockfd, httpMsg.c_str(), httpMsg.length(), 0);
	this->buffer.clear();
	std::cout << "==========================================================\n";
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

std::string	HttpResponse::findValue(std::string key)
{
	std::map<std::string, std::string>::iterator it;

	it = HttpResponse::headers.find(key);
	if (it != headers.end())
		return (it->second);
	return ("");
}

void		HttpResponse::putContentType(std::string filename)
{
	std::string contentType = ResponseConfig::getContentType(filename);
	putHeader("Content-Type", contentType);
}

std::string	HttpResponse::readRangeQuery(std::string range, std::string body)
{
	std::string value1 = range.substr(range.find("="), range.find("-"));
	std::string value2 = range.substr(range.find("-"));

    return (body.substr(std::atoi(value1.c_str()), std::atoi(value2.c_str())));
}

ServerConfiguration	*HttpResponse::getServerConfiguration(void)
{
	return (this->serverConfig);
}

Event	*HttpResponse::getEvent(void)
{
	return (this->event);
}