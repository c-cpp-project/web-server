#include "HttpRequestParser.hpp"
#include "HttpRequestHandler.hpp"
// #include "WebServer.hpp"

void HttpRequestParser::parse(HttpRequest*& request, ServerConfiguration *&server_config, const std::string& buffer)
{
	long start = 0;
	request = new HttpRequest();
	parseRequestLine(request, server_config, buffer, start);
	parseRequestHeaders(request, server_config, buffer, start);
	if (isExistBody(request))
		parseRequestBody(request, server_config, buffer, start);
	parseRequestParams(request);
}

void HttpRequestParser::parseRequestLine(HttpRequest *request, ServerConfiguration *server_config, const std::string& buffer, long& start)
{
	// 요청 라인 전에 들어오는 CRLF는 건너뛰기
	while (buffer.substr(start, 2) == "\r\n")
		start += 2;

	// 요청 라인이 들어왔는지 확인하기
	size_t end_of_line = buffer.find("\r\n", start);
	if (end_of_line == std::string::npos) // 요청 라인의 끝을 식별할 수 없는 경우
	{
		if (buffer.size() >= server_config->getClientHeaderSize())	// 제한된 헤더 크기만큼 충분히 읽었다면
			throw SocketCloseException400();						// -> 너무 긴 요청 라인
		else														// 제한된 요청 라인 길이만큼 충분히 읽지 못했다면
			throw INCOMPLETE_REQUEST;								// -> 불완전한 요청 라인
	}

	// 요청 라인 파싱하기
	std::string line = buffer.substr(0, end_of_line);
	request->setRequestLine(new RequestLine(line));
	start = end_of_line + 2;
}

void HttpRequestParser::parseRequestHeaders(HttpRequest *request, ServerConfiguration *&server_config, const std::string& buffer, long& start)
{
	// 요청 헤더가 들어왔는지 확인하기
	size_t end_of_headers = buffer.find("\r\n\r\n", start);
	if (end_of_headers == std::string::npos) // 요청 헤더의 끝을 식별할 수 없는 경우
	{
		if (buffer.size() >= server_config->getClientHeaderSize())	// 최대 헤더 크기만큼 충분히 읽었다면
			throw SocketCloseException400();						// -> 너무 긴 헤더
		else														// 최대 헤더 크기만큼 충분히 읽지 못했다면
			throw INCOMPLETE_REQUEST;								// 불완전한 요청 라인
	}

	// 요청 헤더 파싱하기
	request->setRequestHeaders(new HttpHeaders());
	while (true)
	{
		int nl_pos = buffer.find("\r\n", start);
		if (nl_pos == start)
			break; // 헤더 부분이 끝난 경우 while문 탈출
		std::string line = buffer.substr(start, nl_pos - start);
		if (request->addHeader(line) == FAILURE) // 한 줄을 파싱하여 헤더에 추가
			throw "400";
		start = nl_pos + 2;
	}
	start += 2;

	if (request->getHeader("Host") == "")
		throw "400"; // HOST 헤더가 없는 경우
}

bool HttpRequestParser::isExistBody(HttpRequest *request)
{
	// POST 요청이 아닌 경우 요청 본문이 없다고 판단하기
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

void HttpRequestParser::parseRequestBody(HttpRequest *request, ServerConfiguration *server_config, const std::string& buffer, long start)
{
	// content_length 구하기
	long content_length = RequestUtility::strToPositiveLong(request->getHeader("Content-Length"));
	if (content_length == FAILURE)
		throw "400"; // content_length 값이 유효하지 않은 경우
	if (content_length > server_config->getClientBodySize(request->getPath()))
		throw SocketCloseException413(); // content_length가 제한된 본문 크기를 초과하는 경우
	if (buffer.size() < start + content_length)
		throw INCOMPLETE_REQUEST; // 버퍼에 content_length 만큼 충분히 없는 경우
	
	// 본문 설정하기
	request->setRequestBody(buffer.substr(start, content_length));
}

void HttpRequestParser::parseRequestParams(HttpRequest *request)
{
	request->setRequestParams(new RequestParams());

	// 요청 라인의 uri 뒤에 붙은 쿼리 스트링 파싱하기
	if (request->addRequestParamsToQueryString(request->getQueryString()) == FAILURE)
		throw "400";

	// 본문의 쿼리 스트링 파싱하기
	if (request->getHeader("Content-Type") == "application/x-www-form-urlencoded" &&
		request->addRequestParamsToQueryString(request->getBody()) == FAILURE)
		throw "400";
}