#include "RequestLine.hpp"
#include <cctype>

RequestLine::RequestLine(const std::string& input)
{
	std::vector<std::string> tockens = HttpRequestUtility::splitString(input, ' ');
	
	// 요청 라인이 "[메서드] [경로] [프로토콜]" 형식이 아닌 경우 400 응답
	if (tockens.size() != 3)
		throw std::invalid_argument("400 Bad Request");

	parseMethod(tockens[0]);
	parseURI(tockens[1]);
	parseProtocol(tockens[2]);
}

void RequestLine::parseMethod(std::string method_string)
{
	// 영어 대문자 이외의 문자가 포함된 메서드인 경우 400 응답
	for(size_t i = 0; i < method_string.size(); i++)
	{
		if (!std::isupper(method_string[i]))
			throw std::invalid_argument("400 Bad Request");
	}

	// 유효하지 않거나 지원하지 않은 메서드인 경우 400 응답
	if (method_string != "GET" && method_string != "POST" && method_string != "DELETE")
		throw std::invalid_argument("400 Bad Request");

	method = method_string;
}

void RequestLine::parseURI(std::string uri_string)
{
	std::vector<std::string> tockens = HttpRequestUtility::splitString(uri_string, '?');
	
	if (tockens.size() < 1 || 2 < tockens.size())
		throw std::invalid_argument("400 Bad Request");
	if (tockens[0][0] != '/')
		throw std::invalid_argument("400 Bad Request");

	path = tockens[0];
	if (tockens.size() == 2)
		query_string = tockens[1];
	
	// TODO : 이후 요청 헤더까지 받고 나서 경로 유효성 검사를 추가로 진행함
}

void RequestLine::parseProtocol(std::string protocol_string)
{
	if (protocol_string != "HTTP/1.1")
		throw std::invalid_argument("400 Bad Request");

// TODO : 테스터기에 따라 추가적인 조건이 더 필요할 가능성 있음
// 	std::vector<std::string> tockens = HttpRequestUtility::splitString(protocol_string, '/')
// 	if (tockens.size() != 2)
// 	{
// 		throw std::invalid_argument("400 Bad Request");
// 	}
// 	if (tockens[0] != "HTTP")
// 	{
// 		throw std::invalid_argument("400 Bad Request");
// 	}
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