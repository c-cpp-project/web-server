#include "HttpRequest.hpp"
#include "RequestUtility.hpp"

std::string HttpRequest::getMethod() const
{
	return (request_line->getMethod());
}

std::string HttpRequest::getPath() const
{
	return (request_line->getPath());
}

std::string HttpRequest::getQueryString() const
{
	return (request_line->getQueryString());
}

std::string HttpRequest::getHeader(const std::string& header) const
{
	return (request_headers->getHeader(header));
}

std::string HttpRequest::getParameter(const std::string& param) const
{
	return (request_params->getParameter(param));
}

std::string HttpRequest::getRequestBody() const
{
	return (request_body);
}

void HttpRequest::setRequestLine(RequestLine *line)
{
	request_line = line;
}

void HttpRequest::setRequestHeaders(HttpHeaders *headers)
{
	request_headers = headers;
}

void HttpRequest::addHeader(const std::string& line)
{
	request_headers->addHeader(line);
}

void HttpRequest::setRequestBody(const std::string& body)
{
	request_body = body;
}

void HttpRequest::setRequestParams(RequestParams *params)
{
	request_params = params;
}


// 테스트용
std::map<std::string, std::string>::iterator HttpRequest::getHeadersBegin() const
{
	return (request_headers->getBegin());
}
std::map<std::string, std::string>::iterator HttpRequest::getHeadersEnd() const
{
	return (request_headers->getEnd());
}
std::map<std::string, std::string>::iterator HttpRequest::getParamsBegin() const
{
	return (request_params->getBegin());
}
std::map<std::string, std::string>::iterator HttpRequest::getParamsEnd() const
{
	return (request_params->getEnd());
}