#include "HttpResponse.hpp"

#include "../Bean/HttpHandler.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(int sockfd) { this->sockfd = sockfd; }

HttpResponse::HttpResponse(int sockfd, ServerConfiguration *serverConfig,
													 Event *event)  // default 지정
{
	this->sockfd = sockfd;
	this->send_timeout = "60";  // nginx send_timeout default
	this->max_size = 64 * K;
	this->status_code = "200";
	this->responseBody = "";
	this->serverConfig = serverConfig;
	this->event = event;
	this->buffer = "";
}

HttpResponse::HttpResponse(int sockfd, int max_size,
													 ServerConfiguration *serverConfig,
													 Event *event)  // tokenizer
{
	this->sockfd = sockfd;
	this->send_timeout = "60";  // nginx send_timeout default
	this->max_size = max_size;
	this->status_code = "200";
	this->serverConfig = serverConfig;
	this->event = event;
	this->buffer = "";
}

HttpResponse::HttpResponse(int sockfd, std::string send_timeout,
													 ServerConfiguration *serverConfig, Event *event) {
	this->sockfd = sockfd;
	this->send_timeout = send_timeout;  // nginx send_timeout default
	this->serverConfig = serverConfig;
	this->event = event;
	this->buffer = "";
}

HttpResponse &HttpResponse::operator=(const HttpResponse &ref) {
	if (this == &ref) return (*this);
	sockfd = ref.sockfd;
	headers = ref.headers;
	buffer = ref.buffer;
	max_size = ref.max_size;
	send_timeout = ref.send_timeout;
	status_code = ref.status_code;
	responseBody = ref.responseBody;
	authenticated = ref.authenticated;
	serverConfig = ref.serverConfig;
	event = ref.event;
	buffer = ref.buffer;
	return (*this);
}

void HttpResponse::putHeader(std::string key, std::string value) {
	headers.insert(std::make_pair(key, value));
}

void HttpResponse::removeHeader(std::string key) {
	headers.erase(key);
	std::cout << "[remove header: " << key << "]\n";
}

void HttpResponse::listDirectory(std::string directory) {
	DIR *dir;
	std::string body;
	struct dirent *entry;
	std::stringstream ss;
	HttpHandler	*httpHandler;

	dir = opendir(directory.c_str());
	if (dir == NULL) {
		std::cout << "Error opening directory\n";
		throw "404";
	}
	body = "";
	while ((entry = readdir(dir)) != NULL) {
		std::cout << entry->d_name << "\n";
		body += std::string("File name : ") + entry->d_name + std::string("\n");
	}
	closedir(dir);
	ss << body.length();
	putHeader("Content-Type", "text/html");
	putHeader("Content-Length", ss.str());
	sendBody(body, true); 
	putHeader("Connection", "keep-alive");
	httpHandler = new HttpHandler(getSockfd(), getByteDump(), serverConfig);
	if (headers.at("connection") == "close")
		httpHandler->setConnectionClose(true);
	else
		httpHandler->setConnectionClose(false);
	event->saveEvent(getSockfd(), EVFILT_WRITE, EV_ENABLE, 0, 0, httpHandler);  // SEND
}

void HttpResponse::redirect(std::string redirectUri) {
	HttpHandler	*httpHandler;
	setStatusCode("302");
	putHeader("Content-Type", "text/html");
	putHeader("Location", redirectUri);
	putHeader("Content-Length", "0");
	sendBody("", true);
	putHeader("Connection", "keep-alive");

	httpHandler = new HttpHandler(getSockfd(), getByteDump(), serverConfig);
	if (headers.at("connection") == "close")
		httpHandler->setConnectionClose(true);
	else
		httpHandler->setConnectionClose(false);
	event->saveEvent(getSockfd(), EVFILT_WRITE, EV_ENABLE, 0, 0, httpHandler);  // SEND
}

void HttpResponse::emptyForward(void) {
	HttpHandler	*httpHandler;
	setStatusCode("200");
	putHeader("Content-Type", "text/html");
	putHeader("Content-Length", "0");
	sendBody("", true);
	putHeader("Connection", "keep-alive");

	httpHandler = new HttpHandler(getSockfd(), getByteDump(), serverConfig);
	if (headers.at("connection") == "close")
		httpHandler->setConnectionClose(true);
	else
		httpHandler->setConnectionClose(false);
	event->saveEvent(getSockfd(), EVFILT_WRITE, EV_ENABLE, 0, 0, httpHandler);  // SEND
}

void  HttpResponse::putHeaders(int length, HttpRequest &request)
{
	std::string 			range;
	std::stringstream 		ss;
	std::string				bodyLength;

	putHeader("Server", serverConfig->getServerName());
	putHeader("Date", ResponseConfig::getCurrentDate());
	if (request.getParameter("Range") != "" && request.getMethod() == "GET")
	{
		std::string value1 = range.substr(range.find("="), range.find("-"));
		std::string value2 = range.substr(range.find("-"));
		length = std::atoi(value2.c_str()) - std::atoi(value1.c_str());
	}
	ss << length;
	bodyLength = ss.str();
	putHeader("Content-Length", bodyLength);
	putHeader("Connection", "keep-alive");
}

void  HttpResponse::forward(HttpRequest &request)  // controller에서 사용한다.
{
	int 		fd;
	std::string uri;
	struct stat buf;
	HttpHandler	*httpHandler;

	uri = request.getPath();
	if (400 <= std::atoi(getStatusCode().c_str()) && std::atoi(getStatusCode().c_str()) < 600)  // fail.page
	{
		uri = serverConfig->getErrorpageResourcePath(std::atoi(getStatusCode().c_str()));
		if (uri == "") 
			uri = "static/html/welcome.html";
	}
	std::cout << "uri: " << uri << '\n';
	fd = open(uri.c_str(), O_RDONLY);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	if (fd < 0)
	{
		std::cout << strerror(errno) << "\n";
		std::cout << fd << ", " << request.getMethod() << ", " << uri << ", " << getStatusCode() << "\n";
		if (access(uri.c_str(), F_OK) == 0)
			return ; // no file descriptor
		close(fd);
		throw "404";
	}
	std::cout << fd << ", " << request.getPath() << ": status code = " << getStatusCode() << "\n";
	stat(uri.c_str(), &buf);
	if (buf.st_size == 0)
	{
		emptyForward();
		return ;
	}
	putHeader("Content-Type", ResponseConfig::getContentType(uri));
	putHeaders(buf.st_size, request);
	if ("400" <= getStatusCode() && getStatusCode() < "600")
		httpHandler = new HttpHandler(fd, *this, buf.st_size);
	else
		httpHandler = new HttpHandler(fd, request, *this, buf.st_size);
	if (headers.find("connection") != headers.end())
		httpHandler->setConnectionClose(headers.at("connection") == "close");
	event->saveEvent(fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, httpHandler);
}

void HttpResponse::sendBody(std::string body, bool isStaticFile)  // api 요청에 대한 응답
{
	std::cout << "isStaticFile: " << isStaticFile << "\n";
	if (isStaticFile == true)
	{
		ResponseStatusLine();
		processHeader();
	}
	else
	{
		size_t		pos;
		std::string	target;
		std::string	replacement = "HTTP/1.1";

		target = "Status:";
		pos = body.find(target);
		if (pos != std::string::npos)
			body = body.replace(pos, target.length(), replacement);
	}
	HttpResponseBody(body);
}

void HttpResponse::ResponseStatusLine() {
	std::string msg = "";

	msg += "HTTP/1.1 ";
	msg += this->status_code + " ";
	msg += ResponseConfig::getHttpStatusMsg(this->status_code);
	msg += "\r\n";
	buffer.append(msg);
}

void HttpResponse::processHeader() {
	std::string msg = "";
	std::map<std::string, std::string>::iterator it;

	for (it = this->headers.begin(); it != this->headers.end(); ++it)
		msg += (it->first + ": " + it->second + "\r\n");
	msg += "\r\n";
	buffer.append(msg);
}

void HttpResponse::HttpResponseBody(std::string body) {
	if (body == "") return;
	buffer.append(body);
}

std::string HttpResponse::getByteDump(void) {
	return (buffer);
}

void HttpResponse::setStatusCode(std::string code) { this->status_code = code; }

std::string HttpResponse::getStatusCode() { return (this->status_code); }

long long HttpResponse::getMaxBodySize() { return (this->max_size); }

int HttpResponse::getSockfd() { return (this->sockfd); }

std::string HttpResponse::findValue(std::string key) {
	std::map<std::string, std::string>::iterator it;

	it = HttpResponse::headers.find(key);
	if (it != headers.end()) return (it->second);
	return ("");
}

void HttpResponse::putContentType(std::string filename) {
	std::string contentType = ResponseConfig::getContentType(filename);
	putHeader("Content-Type", contentType);
}

std::string HttpResponse::readRangeQuery(std::string range, std::string body) {
	std::string value1 = range.substr(range.find("="), range.find("-"));
	std::string value2 = range.substr(range.find("-"));

	return (body.substr(std::atoi(value1.c_str()), std::atoi(value2.c_str())));
}

ServerConfiguration *HttpResponse::getServerConfiguration(void) {
	return (this->serverConfig);
}

Event *HttpResponse::getEvent(void) { return (this->event); }

std::string	HttpResponse::getHeader(std::string key)
{
	if (headers.find(key) != headers.end())
		return (headers.at(key));
	return ("");
}