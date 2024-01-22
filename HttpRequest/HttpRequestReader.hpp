#ifndef HTTP_REQUEST_READER_HPP
# define HTTP_REQUEST_READER_HPP

# define BUF_SIZE 1024

#include <iostream>

class HttpRequestReader
{
	private:
		int socket_fd;
		std::string remainder;

	public:
		HttpRequestReader(int socket_fd);

		std::string getLine();
};

#endif