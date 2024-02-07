#include "RequestParams.hpp"

int RequestParams::addQuearyString(const std::string& query_string)
{
	bool success;
	std::map<std::string, std::string> parsed = RequestUtility::parseQueryString(query_string, success);
	if (success == false)
		return (FAILURE);
	params.insert(parsed.begin(), parsed.end());
	return (SUCCESS);
}

int RequestParams::addCookie(const std::string& cookie_string)
{
	bool success;
	std::map<std::string, std::string> parsed = RequestUtility::parseCookieString(cookie_string, success);
	if (success == false)
		return (FAILURE);
	params.insert(parsed.begin(), parsed.end());
	return (SUCCESS);
}

std::string RequestParams::getParameter(const std::string& param)
{
	if (RequestUtility::isExist(params, param))
		return (params[param]);
	return ("");
}

std::string RequestParams::getCookieValue(const std::string& key)
{
	std::string upper_key = RequestUtility::toUpperString(key);
	return (getParameter(upper_key));
}

// 테스트용
std::map<std::string, std::string>::iterator RequestParams::getBegin()
{
	return (params.begin());
}
std::map<std::string, std::string>::iterator RequestParams::getEnd()
{
	return (params.end());
}