#include "RequestLine.hpp"
#include <cctype>

RequestLine::RequestLine(const std::string& input)
{
	std::vector<std::string> tockens = RequestUtility::splitString(input, ' ');
	
	// 요청 라인이 "[메서드] [경로] [프로토콜]" 형식이 아닌 경우 -> 버퍼 비우기
	if (tockens.size() != 3)
	{
		// TODO : 400 응답 보내고, 버퍼 비우기
		throw "400";
	}

	parseMethod(tockens[0]); // 메서드 파싱
	parseURI(tockens[1]); // 경로 파싱
	parseProtocol(tockens[2]); // 프로토콜 파싱
}

void RequestLine::parseMethod(std::string method_string)
{
	// 영어 대문자 이외의 문자가 포함된 메서드인 경우 -> 버퍼 비우기
	for(size_t i = 0; i < method_string.size(); i++)
	{
		if (!std::isupper(method_string[i]))
		{
			// TODO : 400 응답 보내고, 버퍼 비우기
			throw "400";
		}
	}

	// 유효하지 않거나 지원하지 않은 메서드인 경우 -> 버퍼 비우기
	if (method_string != "GET" && method_string != "POST" && method_string != "DELETE")
	{
		// TODO : 400 응답 보내고, 버퍼 비우기
		throw "400";
	}

	method = method_string;
}

void RequestLine::parseURI(std::string uri_string)
{
	if (uri_string.size() > 8000) // 허용된 URI 길이를 초과하는 경우 414 응답
		throw std::invalid_argument("414 URI Too Long");

	std::vector<std::string> tockens = RequestUtility::splitString(uri_string, '?');
	if (tockens.size() < 1 || 2 < tockens.size())
		throw std::invalid_argument("400 Bad Request");
	if (tockens[0][0] != '/') // 슬래시(/)로 시작하지 않는 경로의 경우 400 응답
		throw std::invalid_argument("400 Bad Request");

	// ?를 기준으로 앞은 path, 뒤는 query_string으로 저장
	path = tockens[0];
	if (tockens.size() == 2)
		query_string = tockens[1];
}

void RequestLine::parseProtocol(std::string protocol_string)
{
	// TODO: 테스트기 돌려본 후 필요하다면 더 세심한 처리 필요
	if (protocol_string != "HTTP/1.1")
		throw std::invalid_argument("400 Bad Request");
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