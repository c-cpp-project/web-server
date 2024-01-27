#include "HttpRequest.hpp"
#include "HttpRequestUtility.hpp"

HttpRequest::HttpRequest(int socket_fd)
{
	reader = new HttpRequestReader(socket_fd);
	parseRequestLine();
	parseRequestHeaders();
	parseRequestParams();
}

void HttpRequest::parseRequestLine()
{
	std::string line;
	while (true) // 요청 라인 전에 여러 개의 빈 줄이 들어올 수 있다.
	{
		line = reader->getLine();
		if (line != "") break;
	}
	request_line = new RequestLine(line);
}

void HttpRequest::parseRequestHeaders()
{
	request_headers = new HttpHeaders();
	while (true)
	{
		std::string line = reader->getLine();
		if (line == "")
			break;

		std::vector<std::string> tockens = HttpRequestUtility::splitString(line, ':');
		if (tockens.size() >= 2)
		{
			request_headers->addHeader(line);
		}
		else // line에 콜론이 포함되지 않은 경우
		{
			// if (헤더 줄에 화이트스페이스가 포함되었는가)
			// 	throw std::invalid_argument("400 Bad Request");
		}
	}
}

void HttpRequest::parseRequestParams()
{
	request_params = new RequestParams();
	// request_line의 query_string이 빈 문자열이어도 안 터질까?
	request_params->addQuearyString(request_line->getQueryString());

	// 본문 형식이 query string 이라면
	if (request_headers->getHeader("Content-Type") == "application/x-www-form-urlencoded")
	{
		request_params->addQuearyString(reader->getLine());
	}
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
std::string HttpRequest::getHeader(std::string header) const
{
	return (request_headers->getHeader(header));
}
std::string HttpRequest::getParameter(std::string param) const
{
	return (request_params->getParameter(param));
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