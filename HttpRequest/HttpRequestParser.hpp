#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include "HttpRequest.hpp"
# include "RequestException.hpp"
#include "../server/ServerConfiguration.hpp"

# define INCOMPLETE_REQUEST 1 // 요청을 더 읽어오자
# define START_CHUNKED_REQUEST 2 // 청크 전송을 시작하는 요청
# define LAST_CHUNKED_REQUEST 3 // 청크 전송을 끝내는 요청

class HttpRequestParser
{
	private:
		static void parseRequestLine(HttpRequest *request, ServerConfiguration *server_config, const std::string& buffer, long& start);
		static void parseRequestHeaders(HttpRequest *request, ServerConfiguration *&server_config, const std::string& buffer, long& start);
		static bool isExistBody(HttpRequest *request);
		static void parseRequestBody(HttpRequest *request, ServerConfiguration *server_config, const std::string& buffer, long start);

	public:
		static void parse(HttpRequest*& request, ServerConfiguration *&server_config, const std::string& buffer);
		static void parseRequestParams(HttpRequest *request);
};

#endif