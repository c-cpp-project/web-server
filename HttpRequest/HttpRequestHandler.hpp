#ifndef HTTP_REQUEST_HANDLER_HPP
# define HTTP_REQUEST_HANDLER_HPP

# include "HttpRequest.hpp"
# include "../server/ServerConfiguration.hpp"

class HttpRequestHandler
{
	private:
		int socket_fd;
		ServerConfiguration server_info;

		static std::map<int, std::string> buffers; // 요청을 읽어오는 소켓, 버퍼
		static std::map<int, HttpRequest*> chunkeds; // chunked 수신 중인 소켓, request 객체

		int readRequest();
		static void removeBuffer(int socket_fd);

	public:
		HttpRequestHandler(int socket_fd, const ServerConfiguration& server_info);

		void handle();
		int ChunkedRequestHandling(HttpRequest* request);
		void errorHandling(const char *erorr_code);

		static HttpRequest *removeChunkedRequest(int socket_fd);
		static void removeAndDeleteChunkedRequest(int socket_fd);
		static HttpRequest *getChunkedRequest(int socket_fd);
		static const std::string& getBuffer(int socket_fd);
		static void addChunkedRequest(int socket_fd, HttpRequest *request);
		static void removePartOfBuffer(int socket_fd, int start, int count);
};

#endif