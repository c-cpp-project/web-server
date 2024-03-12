#ifndef HTTP_REQUEST_FACTORY_HPP
# define HTTP_REQUEST_FACTORY_HPP

# include "HttpRequestParser.hpp"

class HttpRequestFactory
{
	private:
		static long parseChunkedRequest(HttpRequest* request, ServerConfiguration *&server_config, const std::string& buffer);
		static void removeRequestInBuffer(int socket_fd, HttpRequest* request);
		static void SpecialExceptionHandling(const int& e, int socket_fd, HttpRequest*& request);

	public:
		static HttpRequest *create(int socket_fd, ServerConfiguration *&server_config);
};

#endif
