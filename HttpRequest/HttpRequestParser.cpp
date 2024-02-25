#include "HttpRequestParser.hpp"
#include "HttpRequestHandler.hpp"

void HttpRequestParser::parse(int socket_fd, HttpRequest*& request, ServerConfiguration *server_config)
{
	readHeader(socket_fd, server_config);
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);

	int start = 0;
	request = new HttpRequest();
	parseRequestLine(request, buffer, start);
	std::cout << "after parse request line\n";
	parseRequestHeaders(request, buffer, start);
	std::cout << "after parse headers\n";

	if (isExistBody(request))
	{
		parseRequestBody(socket_fd, request, server_config, start);
		std::cout << "after parse body\n";
	}
	parseRequestParams(request);
	std::cout << "after parse params\n";
}

void HttpRequestParser::readHeader(int socket_fd, ServerConfiguration *server_config)
{
	long client_header_size = server_config->getClientHeaderSize();
	HttpRequestHandler::readRequest(socket_fd, client_header_size);
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);

	size_t end_of_headers = buffer.find("\r\n\r\n");
	if (end_of_headers == std::string::npos)
	{
		if (buffer.size() >= server_config->getClientHeaderSize())
			throw SocketCloseException400();
		throw INCOMPLETE_REQUEST;
	}
}

void HttpRequestParser::parseRequestLine(HttpRequest *request, const std::string& buffer, int& start)
{
	// 요청 라인 전에 들어오는 CRLF는 무시하고 정상적으로 처리한다.
	while (buffer.substr(start, 2) == "\r\n")
		start += 2;

	size_t nl_pos = buffer.find("\r\n");
	if (nl_pos == std::string::npos)
		throw "400"; // 요청 라인을 식별할 수 없는 경우 -> 요청 거부
	std::string line = buffer.substr(0, nl_pos);
	request->setRequestLine(new RequestLine(line));
	start = nl_pos + 2;
}

void HttpRequestParser::parseRequestHeaders(HttpRequest *request, const std::string& buffer, int& start)
{
	request->setRequestHeaders(new HttpHeaders());

	while (true)
	{
		int nl_pos = buffer.find("\r\n", start);
		if (nl_pos == start)
			break; // 헤더 부분이 끝남
		std::string line = buffer.substr(start, nl_pos - start);
		if (request->addHeader(line) == FAILURE)
			throw "400";
		start = nl_pos + 2;
	}
	start += 2;

	if (request->getHeader("Host") == "")
		throw "400"; // HOST 헤더가 없는 경우
}

bool HttpRequestParser::isExistBody(HttpRequest *request)
{
	if (request->getMethod() != "POST")
		return (false);

	// chunked 헤더를 포함하는 POST 요청 -> chunked 수신을 시작하는 요청
	if (request->getHeader("Transfer-Encoding") == "chunked") // TODO : chunked 이외는 다 무시하자.. 괜찮나?
	{
		request->setHeader("Content-Length", "0");
		throw START_CHUNKED_REQUEST;
	}
	
	if (request->getHeader("Content-Length") == "")
		throw "411"; // Transfer-Encoding, Content-Length 둘 다 없는 경우 -> 411 응답
	
	return (true);
}

long HttpRequestParser::readBody(int socket_fd, HttpRequest *request, ServerConfiguration *server_config)
{
	long client_body_size = server_config->getClientBodySize(request->getPath());
	long content_length = RequestUtility::strToPositiveLong(request->getHeader("Content-Length"));
	if (content_length == FAILURE)
		throw "400";
	if (content_length > client_body_size)
		throw SocketCloseException400();
	HttpRequestHandler::readRequest(socket_fd, client_body_size);
	return (content_length);
}

void HttpRequestParser::parseRequestBody(int socket_fd, HttpRequest *request, ServerConfiguration *server_config, int start)
{
	long content_length = readBody(socket_fd, request, server_config);
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);

	// content-length 만큼 본문이 없는 경우 -> 불완전한 요청
	if ((long)buffer.size() <= start + content_length)
		throw INCOMPLETE_REQUEST;

	// TODO : Content-Type이 없는 경우, 강제로 쿼리스트링으로 인식해도 될까?
	if (request->getHeader("Content-Type") == "")
		request->setHeader("Content-Type", "application/x-www-form-urlencoded");
	request->setRequestBody(buffer.substr(start, content_length));
}

void HttpRequestParser::parseRequestParams(HttpRequest *request)
{
	request->setRequestParams(new RequestParams());
	if (request->addRequestParamsToQueryString(request->getQueryString()) == FAILURE)
		throw "400";
	if (request->getHeader("Content-Type") == "application/x-www-form-urlencoded")
	{
		if (request->addRequestParamsToQueryString(request->getBody()) == FAILURE)
			throw "400";
	}
}