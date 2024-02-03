#include "HttpRequest.hpp"
#include "RequestUtility.hpp"

HttpRequest::HttpRequest()
{
	request_line = NULL;
	request_headers = NULL;
	request_params = NULL;
}

HttpRequest::~HttpRequest()
{
	delete request_line;
	delete request_headers;
	delete request_params;
}

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

std::string HttpRequest::getBody() const
{
	return (request_body);
}

void HttpRequest::setRequestLine(RequestLine *line)
{
	request_line = line;
}

void HttpRequest::setMethod(const std::string& method)
{
	request_line->setMethod(method);
}

void HttpRequest::setPath(const std::string& path)
{
	request_line->setPath(path);
}

void HttpRequest::setRequestHeaders(HttpHeaders *headers)
{
	request_headers = headers;
}

int HttpRequest::addHeader(const std::string& line)
{
	return (request_headers->addHeader(line));
}

void HttpRequest::setHeader(const std::string& field, const std::string& value)
{
	request_headers->setHeader(field, value);
}

void HttpRequest::setRequestBody(const std::string& body)
{
	request_body = body;
}

void HttpRequest::addRequestBody(const std::string& body)
{
	request_body += body;
}

void HttpRequest::setRequestParams(RequestParams *params)
{
	request_params = params;
}

int HttpRequest::addRequestParamsToQueryString(const std::string& query_string)
{
	return (request_params->addQuearyString(query_string));
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