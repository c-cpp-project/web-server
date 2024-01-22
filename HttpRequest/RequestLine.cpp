#include "RequestLine.hpp"

RequestLine::RequestLine(const std::string& input)
{
	std::vector<std::string> tockens = HttpRequestUtility::splitString(input, ' ');
	if (tockens.size() != 3)
	{
		throw std::invalid_argument("Request Line's tocken count is invalid.");
	}

	parseMethod(tockens[0]);
	parseURI(tockens[1]);
	if (tockens[2] != "HTTP/1.1")
	{
		throw std::invalid_argument("Request Line's protocol version is invalid.");
	}
}

void RequestLine::parseMethod(std::string method_string)
{
	if (method_string == "GET" || method_string == "POST" || method_string == "DELETE")
	{
		method = method_string;
	}
	else
	{
		throw std::invalid_argument("Request Line's method tocken is invalid.");
	}
}

void RequestLine::parseURI(std::string uri_string)
{
	std::vector<std::string> tockens = HttpRequestUtility::splitString(uri_string, '?');
	if (tockens.size() == 1)
	{
		path = tockens[0];
		query_string = "";
	}
	else if (tockens.size() == 2)
	{
		path = tockens[0];
		query_string = tockens[1];
	}
	else
	{
		throw std::invalid_argument("Request Line's uri is invalid.");
	}

	// path 유효성 검사
	// query_string은 아마 RequestParams 쪽에서 유효성 검사할 듯
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