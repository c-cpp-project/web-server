#include "HttpHeaders.hpp"

int HttpHeaders::addHeader(const std::string& line)
{
	// 헤더 필드와 헤더 값을 적절히 분리
	std::string field, value;
	size_t colon_pos = line.find(':');
	if (colon_pos == std::string::npos){
		field = RequestUtility::toUpperString(line);
	} else {
		field = RequestUtility::toUpperString(line.substr(0, colon_pos));
		value = RequestUtility::trim(line.substr(colon_pos + 1, line.size() - colon_pos - 1), " ");
	}

	if (RequestUtility::containWhiteSpace(field)) // 헤더 필드에 화이트스페이스가 포함되면 400 응답
		return (FAILURE);
	if (RequestUtility::isExist(headers, field)) // headers에 이미 존재하는 헤더 필드라면
	{
		if (strongDuplicationBan(field)) // 중복이 아예 허용되지 않는 헤더 필드라면 400 응답
			return (FAILURE);
		if (weakDuplicationBan(field)) // 중복이 들어와도 첫 값을 유지해야 하는 헤더 필드라면 넘어가기
			return (SUCCESS);
		headers[field] += ", " + value;
	}
	else // headers에 존재하지 않는 헤더 필드라면
	{
		headers[field] = value;
	}
	return (SUCCESS);
}

std::string HttpHeaders::getHeader(const std::string& field)
{
	std::string upper_field = RequestUtility::toUpperString(field);
	if (RequestUtility::isExist(headers, upper_field))
		return (headers[upper_field]);
	return ("");
}

void HttpHeaders::setHeader(const std::string& field, const std::string& value)
{
	std::string upper_field = RequestUtility::toUpperString(field);
	headers[upper_field] = value;
}

void HttpHeaders::removeHeader(const std::string& field)
{
	std::string upper_field = RequestUtility::toUpperString(field);
	std::map<std::string, std::string>::iterator it = headers.find(upper_field);
	if (it != headers.end())
		headers.erase(it);
}

// 중복이 들어와도 첫 값을 유지하는 헤더들
bool HttpHeaders::weakDuplicationBan(const std::string& field)
{
	if (field == "RANGE" || field == "CONTENT-TYPE" || field == "CONTENT-ENCODING" || field == "CONTENT-LANGUAGE" || field == "DATE")
		return (true);
	return (false);
}

// 중복이 들어오는 순간 400 응답을 보내는 헤더들
bool HttpHeaders::strongDuplicationBan(const std::string& field)
{
	if (field == "HOST" || field == "AUTHORIZATION" || field == "EXPECT"
		|| field == "CONTENT-LENGTH" || field == "TRANSFER_ENCODING"
		|| field == "IF-MODIFIED-SINCE" || field == "IF-UNMODIFIED-SINCE" || field == "IF-MATCH" || field == "IF-NONE-MATCH" || field == "IF-RANGE")
			return (true);
	return (false);
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