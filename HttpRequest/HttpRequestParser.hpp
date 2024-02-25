#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include "HttpRequest.hpp"
# include "SocketCloseException.hpp"
#include "../server/ServerConfiguration.hpp"

# define INCOMPLETE_REQUEST 1 // 요청을 더 읽어오자
# define START_CHUNKED_REQUEST 2 // 청크 전송을 시작하는 요청
# define LAST_CHUNKED_REQUEST 3 // 청크 전송을 끝내는 요청

class HttpRequestParser
{
	private:
		static void readHeader(int socket_fd, ServerConfiguration *server_config);
		static void parseRequestLine(HttpRequest *request, const std::string& buffer, int& start);
		static void parseRequestHeaders(HttpRequest *request, const std::string& buffer, int& start);
		static bool isExistBody(HttpRequest *request);
		static long readBody(int socket_fd, HttpRequest *request, ServerConfiguration *server_config, int start);
		static void parseRequestBody(int socket_fd, HttpRequest *request, ServerConfiguration *server_config, int start);

	public:
		static void parse(int socket_fd, HttpRequest*& request, ServerConfiguration *server_config);
		static void parseRequestParams(HttpRequest *request);
};

#endif