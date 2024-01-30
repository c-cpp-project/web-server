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
		std::string getBytes(size_t byte);
		bool readAll();
};

#endif

// GET / HTTP/1.1
// host: 1234
// aaa: test
// test: test

// abcd=1234&abc=123