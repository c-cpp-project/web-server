#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include "HttpRequest.hpp"

# define INCOMPLETE_REQUEST 1 // 요청을 더 읽어오자
# define START_CHUNKED_REQUEST 2 // 청크 전송을 시작하는 요청
# define BAD_REQUEST 400 // 400 응답으로 현재 요청을 거부하자
# define LENGTH_REQUIRED 411 // 411 응답으로 현재 요청을 거부하자

class HttpRequestParser
{
	private:
		static void parseRequestLine(HttpRequest *request, const std::string& buffer, int& start);
		static void parseRequestHeaders(HttpRequest *request, const std::string& buffer, int& start);
		static void parseRequestBody(HttpRequest *request, const std::string& buffer, int start);
		static void parseRequestParams(HttpRequest *request);

	public:
		static HttpRequest *parse(const std::string& buffer, HttpRequest *request);
};

#endif