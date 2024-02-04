#ifndef HTTP_REQUEST_FACTORY_HPP
# define HTTP_REQUEST_FACTORY_HPP

# include "HttpRequestParser.hpp"

class HttpRequestFactory
{
	private:
		static void parseChunkedRequest(int socket_fd, HttpRequest*& request);
		static void removeRequestInBuffer(int socket_fd, HttpRequest* request);
		static void handlingSpecialException(const int& e, int socket_fd, HttpRequest*& request);

	public:
		static HttpRequest *create(int socket_fd);
};

#endif
