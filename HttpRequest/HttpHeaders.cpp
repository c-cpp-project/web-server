#include "HttpHeaders.hpp"

void HttpHeaders::addHeader(const std::string& line)
{
	std::vector<std::string> tockens = HttpRequestUtility::splitString(line, ':');
	if (tockens.size() != 2)
	{
		throw std::invalid_argument("invalid header \"" + line + "\"");
	}

	// 중복 헤더 검사
	// key와 value 유효성 검사
	// value 선행, 후행 화이트스페이스 제거

	std::string key = HttpRequestUtility::trim(tockens[0]);
	std::string value = HttpRequestUtility::trim(tockens[1]);
	headers[key] = value;
}

std::string HttpHeaders::getHeader(const std::string& header)
{
	return (headers[header]);
}

// 테스트용
std::map<std::string, std::string>::iterator HttpHeaders::getBegin()
{
	return (headers.begin());
}
std::map<std::string, std::string>::iterator HttpHeaders::getEnd()
{
	return (headers.end());
}