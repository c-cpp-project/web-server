#ifndef HTTP_REQUEST_HANDLER_HPP
# define HTTP_REQUEST_HANDLER_HPP

# include "HttpRequestParser.hpp"

class HttpRequestHandler
{
	private:
		std::map<int, std::string> buffers; // 요청을 읽어오는 소켓, 버퍼
		std::map<int, HttpRequest*> chunkeds; // chunked 수신 중인 소켓, request 객체

		int readAll(int socket_fd);
		void receiveChunk(const std::string& buffer, HttpRequest *request);
		int convertHexToDecimal(const std::string &hex_string);

		void processIncompleteRequest(HttpRequest *request);
		void processStartChunkedRequest(int socket_fd, HttpRequest *request);
		void processWrongChunkedRequest(int socket_fd);
		void processWrongRequestUnknownRange(int socket_fd, HttpRequest *request);
		void processWrongRequestKnownRange(int socket_fd, HttpRequest *request);

	public:
		void run(int socket_fd);
};

#endif
