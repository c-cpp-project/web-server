#ifndef HTTP_REQUEST_HANDLER_HPP
# define HTTP_REQUEST_HANDLER_HPP

# include "HttpRequestParser.hpp"

class HttpRequestHandler
{
	private:
		std::map<int, std::string> buffers; // 요청을 읽어오는 소켓, 버퍼
		std::map<int, HttpRequest*> chunkeds; // chunked 수신 중인 소켓, request 객체

		HttpRequest *parseRequestInBuffer(int socket_fd);
		int readRequest(int socket_fd);
		HttpRequest *getRequestInChunkeds(int socket_fd);
		HttpRequest *parseChunkedRequest(int socket_fd, HttpRequest *request);
		void removeInBuffers(int socket_fd);
		void removeInChunkeds(int socket_fd);

	public:
		HttpRequest *run(int socket_fd);
};

#endif
