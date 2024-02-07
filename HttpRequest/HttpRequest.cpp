#include "HttpRequest.hpp"
#include "RequestUtility.hpp"

HttpRequest::HttpRequest()
{
	request_line = NULL;
	request_headers = NULL;
	cookie = NULL;
	request_params = NULL;
}

HttpRequest::HttpRequest(const HttpRequest& ref)
{
	*this = ref;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& ref)
{
	request_line = new RequestLine(*ref.request_line);
	request_headers = new HttpHeaders(*ref.request_headers);
	cookie = new RequestParams(*ref.cookie);
	request_body = ref.request_body;
	request_params = new RequestParams(*ref.request_params);
	return (*this);
}

HttpRequest::~HttpRequest()
{
	delete request_line;
	delete request_headers;
	delete cookie;
	delete request_params;
}

std::string HttpRequest::getMethod() const
{
	if (request_line == NULL)
		return ("");
	return (request_line->getMethod());
}

std::string HttpRequest::getPath() const
{
	if (request_line == NULL)
		return ("");
	return (request_line->getPath());
}

std::string HttpRequest::getQueryString() const
{
	if (request_line == NULL)
		return ("");
	return (request_line->getQueryString());
}

std::string HttpRequest::getHeader(const std::string& header) const
{
	if (request_headers == NULL)
		return ("");
	return (request_headers->getHeader(header));
}

std::string HttpRequest::getCookie(const std::string& key) const
{
	if (cookie == NULL)
		return ("");
	return (cookie->getCookieValue(key));
}

std::string HttpRequest::getBody() const
{
	return (request_body);
}

std::string HttpRequest::getParameter(const std::string& param) const
{
	if (request_params == NULL)
		return ("");
	return (request_params->getParameter(param));
}

void HttpRequest::setRequestLine(RequestLine *line)
{
	if (request_line != NULL)
		delete request_line;
	request_line = line;
}

void HttpRequest::setMethod(const std::string& method)
{
	if (request_line != NULL)
		request_line->setMethod(method);
}

void HttpRequest::setPath(const std::string& path)
{
	if (request_line != NULL)
		request_line->setPath(path);
}

void HttpRequest::setRequestHeaders(HttpHeaders *headers)
{
	if (request_headers != NULL)
		delete request_headers;
	request_headers = headers;
}

int HttpRequest::addHeader(const std::string& line)
{
	if (request_headers == NULL)
		return (FAILURE);
	return (request_headers->addHeader(line));
}

void HttpRequest::setHeader(const std::string& field, const std::string& value)
{
	if (request_headers != NULL)
		request_headers->setHeader(field, value);
}

void HttpRequest::removeHeader(const std::string& field)
{
	if (request_headers != NULL)
		request_headers->removeHeader(field);
}

void HttpRequest::setCookie(RequestParams *_cookie)
{
	if (cookie != NULL)
		delete cookie;
	cookie = _cookie;
}

int HttpRequest::addRequestParamsToCookie(const std::string& cookie_string)
{
	if (cookie == NULL)
		return (FAILURE);
	return (cookie->addCookie(cookie_string));
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
	if (request_params == NULL)
		return (FAILURE);
	return (request_params->addQuearyString(query_string));
}

// 테스트용
void HttpRequest::printAllHeader() const
{
	if (request_headers == NULL)
		return;
	std::map<std::string, std::string>::iterator it;
	for(it = request_headers->getBegin(); it != request_headers->getEnd(); it++)
		std::cout << it->first << ": " << it->second << "\n";
}

void HttpRequest::printAllParams() const
{
	if (request_params == NULL)
		return;
	std::map<std::string, std::string>::iterator it;
	for(it = request_params->getBegin(); it != request_params->getEnd(); it++)
		std::cout << it->first << ": " << it->second << "\n";
}