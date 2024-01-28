#include "HttpHeaders.hpp"

bool HttpHeaders::isExist(const std::string& field)
{
	if (headers.find(field) == headers.end())
		return (false);
	return (true);
}

void HttpHeaders::addHeader(const std::string& line)
{
	// 헤더 필드와 헤더 값을 적절히 분리
	std::string field, value;
	size_t colon_pos = line.find(':');
	if (colon_pos == std::string::npos) {
		field = HttpRequestUtility::toUpperString(line);
		value = "";
	} else {
		field = HttpRequestUtility::toUpperString(line.substr(0, colon_pos));
		value = HttpRequestUtility::trim(line.substr(colon_pos + 1, line.size() - colon_pos - 1), " ");
	}

	// 예외 처리
	if (HttpRequestUtility::containWhiteSpace(field)) // 화이트스페이스가 포함된 헤더 필드
		throw std::invalid_argument("400 Bad Request");
	if (isExist(field) && HttpRequestUtility::isImpossibleDuplication(field)) // 허용되지 않는 중복 헤더 필드
		throw std::invalid_argument("400 Bad Request");
	// TODO : 값 유효성 검사가 필요한 일부 필드에 대해 처리
	headers[field] = value;
}

std::string HttpHeaders::getHeader(const std::string& field)
{
	if (isExist(field))
		return (headers[field]);
	return ("");
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