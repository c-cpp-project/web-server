#include "HttpRequest.hpp"
#include "RequestUtility.hpp"

HttpRequest::HttpRequest(int socket_fd)
{
	reader = new HttpRequestReader(socket_fd);
	parseRequestLine(); // 요청 라인 파싱
	parseRequestHeaders(); // 요청 헤더 파싱
	parseRequestParams(); // 요청 본문 파싱 (+ URI에 뒤에 붙는 쿼리스트링 파싱)
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
		if (line == "") // 빈 줄이 나올 때까지 반복
			break;
		request_headers->addHeader(line);
	}

	if (request_headers->getHeader("HOST") == "") // HOST 헤더는 필수, 없다면 400 응답
		throw std::invalid_argument("400 Bad Request");
}

void HttpRequest::parseRequestParams()
{
	request_params = new RequestParams();
	request_params->addQuearyString(request_line->getQueryString());

	if (getMethod() == "POST") // TODO : 요청 메서드가 POST인 경우에만 본문을 처리해도 될까?
	{
		if (getHeader("TRANSFER_ENCODING") != "") // Transfer-Encoding 헤더가 있는 경우
		{
			// TODO : Transfer-Encoding이 chunked인 경우 처리하기
		}
		else if (getHeader("CONTENT-LENGTH") != "") // Content-Length 헤더가 있는 경우
		{
			// Content-Type이 쿼리 스트링이 아닌 경우 415 응답
			if (getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded" && getHeader("CONTENT-LENGTH") != "")
				throw std::invalid_argument("415 Unsupported Media Type");

			size_t body_size = RequestUtility::toPositiveInt(getHeader("CONTENT-LENGTH"));
			std::string body = reader->getBytes(body_size); // Content-Length 값 만큼 본문 읽어오기
			// if (body.size() != body_size || !reader->readAll()) // 소켓 fd에 남은 크기가 Content-Length 값과 일치하지 않으면 400 응답
			// 	throw std::invalid_argument("400 Bad Request"); // TODO : content-length 값이 안 맞으면 400 응답해버리는 게 과연 맞을까...?
			request_params->addQuearyString(body);
		}
		else // Transfer-Encoding, Content-Length 두 헤더 모두 없는 경우
		{
			// TODO : Content-Length가 없는 경우, 그냥 끝까지 읽어버리는 수도 있다. 뭐가 적절할지 더 고민하자.
			throw std::invalid_argument("411 Length Required");
		}
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