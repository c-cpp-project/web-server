#include "HttpHeaders.hpp"

void HttpHeaders::addHeader(const std::string& line)
{
	// 헤더 필드와 헤더 값을 적절히 분리
	std::string field, value;
	size_t colon_pos = line.find(':');
	if (colon_pos == std::string::npos) {
		field = RequestUtility::toUpperString(line);
		value = "";
	} else {
		field = RequestUtility::toUpperString(line.substr(0, colon_pos));
		value = RequestUtility::trim(line.substr(colon_pos + 1, line.size() - colon_pos - 1), " ");
	}

	if (RequestUtility::containWhiteSpace(field)) // 헤더 필드에 화이트스페이스가 포함되면 400 응답
		throw std::invalid_argument("400 Bad Request");
	if (RequestUtility::isExist(headers, field)) // headers에 이미 존재하는 헤더 필드라면
	{
		if (strongDuplicationBan(field)) // 중복이 아예 허용되지 않는 헤더 필드라면 400 응답
			throw std::invalid_argument("400 Bad Request");
		if (weakDuplicationBan(field)) // 중복이 들어와도 첫 값을 유지해야 하는 헤더 필드라면 넘어가기
			return;
		// TODO : 여기서 특정 헤더에 대해 값 유효성 검사가 필요할 수 있다. Host 등
		headers[field] += ", " + value;
	}
	else // headers에에 존재하지 않는 헤더 필드라면
	{
			// TODO : 여기서 특정 헤더에 대해 값 유효성 검사가 필요할 수 있다. Host 등
			headers[field] = value;
	}
}

std::string HttpHeaders::getHeader(const std::string& field)
{
	if (RequestUtility::isExist(headers, field))
		return (headers[field]);
	return ("");
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

void HttpHeaders::setHeader(const std::string& field, const std::string& value)
{
    std::string upper_field = RequestUtility::toUpperString(field);
    headers[upper_field] = value;
}