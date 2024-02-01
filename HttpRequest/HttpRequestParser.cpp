#include "HttpRequestParser.hpp"

HttpRequest *HttpRequestParser::parse(const std::string& buffer)
{
	HttpRequest *request;

	int start = 0;
	request->setRequestLine(parseRequestLine(buffer, start));
	request->setRequestHeaders(parseRequestHeaders(buffer, start));
	request->setRequestBody(parseRequestBody(*request, buffer, start));
	request->setRequestParams(parseRequestParams(*request));

	return (request);
}

// TODO : nginx는 요청 라인 전 일부 개행이 들어와도 무시하고 정상적으로 처리한다.
// 그러나 그런 상황은 프로토콜 위반으로 보는 것이 표준이라고 한다. (일단 이 방식으로 코드를 짰다.)
RequestLine *HttpRequestParser::parseRequestLine(const std::string& buffer, int& start)
{
	// 1. 요청 라인까지의 크기 파악하기
	size_t nl_pos = buffer.find("\r\n");
	if (nl_pos == std::string::npos)
		throw WRONG_REQUEST;
	if (nl_pos > 1000) // TODO : nginx 설정에 따라 최대 헤더 크기를 변경하기
		throw WRONG_REQUEST;
	start = nl_pos + 2;

	// 2. 요청 라인 파싱하기
	std::string line = buffer.substr(0, nl_pos);
	return (new RequestLine(line));
}

HttpHeaders *HttpRequestParser::parseRequestHeaders(const std::string& buffer, int& start)
{
	HttpHeaders *headers = new HttpHeaders();

	while (true)
	{
		int nl_pos = buffer.find("\r\n", start);
		if (nl_pos == start)
			break; // 헤더 부분이 끝남
		std::string line = buffer.substr(start, nl_pos - start);
		headers->addHeader(line);
		start = nl_pos + 2;
		if (start > 1000) // TODO : nginx 설정에 따라 최대 헤더 크기를 변경하기
			throw WRONG_REQUEST;
	}
	start += 2;

	if (headers->getHeader("Host") == "") // HOST 헤더는 필수
		throw WRONG_REQUEST;

	return (headers);
}

std::string HttpRequestParser::parseRequestBody(HttpRequest& request, const std::string& buffer, int start)
{
	if (request.getMethod() != "POST")
		return;
	
	// chunked 헤더를 포함하는 POST 요청 -> chunked 수신을 시작하는 요청
	if (request.getHeader("Transfer-Encoding") == "chunked") // TODO : chunked 이외는 다 무시할까 생각 중
		throw START_CHUNKED_REQUEST;
	
	// Transfer-Encoding과 Content-Length 헤더 둘 다 없는 경우 -> 잘못된 요청
	if (request.getHeader("Content-Length") == "") // Transfer-Encoding, Content-Length 둘 다 없는 경우 오류
		throw WRONG_REQUEST; // 411 Length Required
	
	// content-length 만큼 본문이 없는 경우 -> 불완전한 요청
	size_t content_length = RequestUtility::toPositiveInt(request.getHeader("Content-Length"));
	if (buffer.size() < start + content_length)
		throw INCOMPLETE_REQUEST;
	
	std::string content_type = request.getHeader("Content-Type");
	// TODO : 추후 content-type에 대한 세심한 파싱이 필요할지도!
	// form-data는 boundary도 헤더 값에 같이 가지고 있다!!! 😭
	if (content_type.find("multipart/form-data") == std::string::npos)
		return (parseFormData(request, buffer, start));

	return (buffer.substr(0, content_length));
}

RequestParams *HttpRequestParser::parseRequestParams(const HttpRequest& request)
{
	RequestParams *params = new RequestParams();
	params->addQuearyString(request.getQueryString());

	if (request.getHeader("Content-Type") == "application/x-www-form-urlencoded")
		params->addQuearyString(request.getRequestBody());
	
	return (params);
}

// TODO : 하위 헤더 값의 예외 처리가 완벽하지 않음
// TODO : 여러 개의 데이터가 들어와도 한 개만 처리할 수 있도록 해두었음
std::string HttpRequestParser::parseFormData(HttpRequest& request, const std::string& buffer, int start)
{
	// 두 번째 boundary를 발견할 때까지 읽을거야.

	// 1. boundary 추출하기
	std::string content_type = request.getHeader("Content-Type");
	size_t boundary_pos = content_type.find("boundary=");
	if (boundary_pos == std::string::npos)
		throw WRONG_REQUEST;
	std::string boundary = content_type.substr(boundary_pos + 9);

	if (buffer.substr(start, boundary.size()) != boundary)
		throw WRONG_REQUEST;
	start += boundary.size();
	
	std::map<std::string, std::string> sub_headers;
	while (true)
	{
		size_t nl_pos = buffer.find("\r\n", start);
		if (nl_pos == start)
			break; // 하위 헤더 부분이 끝났다.
		if (nl_pos == std::string::npos)
			throw WRONG_REQUEST;

		std::string line = buffer.substr(start, nl_pos - start);
		request.addHeader(line);
		// TODO : Content-type, Content-disposition 이외의 헤더를 막는 처리가 안 되어 있다.
		// size_t colon_pos = line.find(':');
		// std::string field = RequestUtility::toUpperString(line.substr(start, colon_pos - start));
		// if (field == "CONTENT-TYPE" || field == "CONTENT-DISPOSITION")
		// 	sub_headers[field] = buffer.substr(colon_pos + 1, line.size() - colon_pos - 1);
		// start = nl_pos + 2;
	}
	start += 2;
	size_t next_boundary_pos = buffer.find(boundary, start);
	if (next_boundary_pos == std::string::npos)
		throw WRONG_REQUEST;
	if (buffer[next_boundary_pos - 2] != '\r' || buffer[next_boundary_pos - 1] != '\n')
		throw WRONG_REQUEST;
	return (buffer.substr(start, next_boundary_pos - start - 2));
}