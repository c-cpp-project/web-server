#include "RequestLine.hpp"
#include <cctype>
#include "RequestException.hpp"

RequestLine::RequestLine(const std::string& input)
{
	std::vector<std::string> tockens = RequestUtility::splitString(input, ' ');
	if (tockens.size() != 3)
		throw EscapeHandleException400(); // "[메서드] [경로] [프로토콜]" 형식이 아닌 경우

	parseMethod(tockens[0]); // 메서드 파싱
	parseURI(tockens[1]); // 경로 파싱
	parseProtocol(tockens[2]); // 프로토콜 파싱
}

RequestLine::RequestLine(const RequestLine& ref)
{
	method = ref.getMethod();
	path = ref.getPath();
	query_string = ref.getQueryString();
	protocol_string = ref.getProtocolString();
}

void RequestLine::parseMethod(std::string method_string)
{
	for(size_t i = 0; i < method_string.size(); i++) {
		if (!std::isupper(method_string[i]))
			throw EscapeHandleException400(); // 메서드에 영어 대문자 이외의 문자가 포함된 경우
	}
	method = method_string;
}

void RequestLine::parseURI(std::string uri_string)
{
	std::vector<std::string> tockens = RequestUtility::splitString(uri_string, '?');
	if (tockens.size() < 1 || 2 < tockens.size())
		throw EscapeHandleException400();
	if (tockens[0][0] != '/')
		throw EscapeHandleException400(); // 슬래시(/)로 시작하지 않는 경로

	// ?를 기준으로 앞은 path, 뒤는 query_string으로 저장
	path = tockens[0];
	if (tockens.size() == 2)
		query_string = tockens[1];
}

void RequestLine::parseProtocol(std::string protocol_string)
{
	if (protocol_string != "HTTP/1.1")
		throw EscapeHandleException400();
	this->protocol_string = protocol_string;
}

std::string RequestLine::getMethod() const
{
	return (method);
}

std::string RequestLine::getPath() const
{
	return (path);
}

std::string RequestLine::getQueryString() const
{
	return (query_string);
}

std::string RequestLine::getProtocolString() const
{
	return (protocol_string);
}

void RequestLine::setMethod(const std::string& _method)
{
	method = _method;
}

void RequestLine::setPath(const std::string& _path)
{
	path = _path;
}

void RequestLine::setQueryString(std::string querystring)
{
	this->query_string = querystring;
}