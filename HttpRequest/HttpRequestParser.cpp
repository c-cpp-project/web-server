#include "HttpRequestParser.hpp"
#include "SocketCloseException.hpp"

HttpRequest *HttpRequestParser::parse(const std::string& buffer, HttpRequest *request)
{
	size_t end_of_headers = buffer.find("\r\n\r\n");
	if (end_of_headers == std::string::npos)
		throw INCOMPLETE_REQUEST; // 헤더의 끝을 식별할 수 없는 경우 -> 불완전한 요청
	if (end_of_headers > 1000) // TODO : nginx 설정에 따라 헤더의 최대 크기 변경
		throw SocketCloseException400();

	int start = 0;
	HttpRequest *request = new HttpRequest();
	parseRequestLine(request, buffer, start);
	parseRequestHeaders(request, buffer, start);
	parseRequestBody(request, buffer, start);
	parseRequestParams(request);

	return (request);
}

// TODO : nginx는 요청 라인 전 일부 개행이 들어와도 무시하고 정상적으로 처리한다.
// 그러나 그런 상황은 프로토콜 위반으로 보는 것이 표준이라고 한다. (일단 이 방식으로 코드를 짰다.)
void HttpRequestParser::parseRequestLine(HttpRequest *request, const std::string& buffer, int& start)
{
	// 1. 요청 라인 식별하기
	size_t nl_pos = buffer.find("\r\n");
	if (nl_pos == std::string::npos)
		throw BAD_REQUEST; // 요청 라인을 식별할 수 없는 경우 -> 요청 거부
	start = nl_pos + 2;
	// 2. 요청 라인 파싱하기
	std::string line = buffer.substr(0, nl_pos);
	request->setRequestLine(new RequestLine(line));
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
			throw BAD_REQUEST;
		start = nl_pos + 2;
	}
	start += 2;

	if (request->getHeader("Host") == "")
		throw "400"; // HOST 헤더가 없는 경우
}

void HttpRequestParser::parseRequestBody(HttpRequest *request, const std::string& buffer, int start)
{
	if (request->getMethod() != "POST")
		return;

	// chunked 헤더를 포함하는 POST 요청 -> chunked 수신을 시작하는 요청
	if (request->getHeader("Transfer-Encoding") == "chunked") // TODO : chunked 이외는 다 무시할까 생각 중
	{
		request->setHeader("Content-Length", "0");
		throw START_CHUNKED_REQUEST;
	}
	
	if (request->getHeader("Content-Length") == "")
		throw LENGTH_REQUIRED; // Transfer-Encoding, Content-Length 둘 다 없는 경우 -> 411 응답
	
	// content-length 만큼 본문이 없는 경우 -> 불완전한 요청
	size_t content_length = RequestUtility::strToPositiveInt(request->getHeader("Content-Length"));
	if (buffer.size() < start + content_length)
		throw INCOMPLETE_REQUEST;
	
	// TODO : Content-Type이 없는 경우에는 따로 예외 처리를 안 해도 될까?
	request->setRequestBody(buffer.substr(start, content_length));
}

void HttpRequestParser::parseRequestParams(HttpRequest *request)
{
	request->setRequestParams(new RequestParams());
	if (request->addRequestParamsToQueryString(request->getQueryString()) == FAILURE)
		throw BAD_REQUEST;
	if (request->getHeader("Content-Type") == "application/x-www-form-urlencoded")
	{
		if (request->addRequestParamsToQueryString(request->getBody()) == FAILURE)
			throw BAD_REQUEST;
	}
}