#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include "HttpRequest.hpp"

# define INCOMPLETE_REQUEST 0 // 불완전한 요청
# define START_CHUNKED_REQUEST  1 // chunked 수신을 시작하는 요청
# define WRONG_CHUNKED_REQUEST 2 // 잘못된 chunked 요청
# define WRONG_REQUEST_UNKNOWN_RANGE 3 // 범위를 모르는 잘못된 일반 요청
# define WRONG_REQUEST_KNOWN_RANGE 4 // 범위를 아는 잘못된 일반 요청

class HttpRequestParser
{
	private:
		static RequestLine *parseRequestLine(const std::string& buffer, int& start);
		static HttpHeaders *parseRequestHeaders(const std::string& buffer, int& start);
		static std::string parseRequestBody(HttpRequest& request, const std::string& buffer, int start);
		static std::string parseFormData(HttpRequest& request, const std::string& buffer, int start);
		static RequestParams *parseRequestParams(const HttpRequest& request);

	public:
		static HttpRequest *parse(const std::string& buffer);
};

#endif