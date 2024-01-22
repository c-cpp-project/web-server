#include "HttpRequestReader.hpp"
#include <string>
#include <sys/socket.h>

HttpRequestReader::HttpRequestReader(int socket_fd)
	: socket_fd(socket_fd)
{

}

std::string HttpRequestReader::getLine()
{
	char buffer[BUF_SIZE];
	std::string result = remainder;

	while (true)
	{
		size_t nl_pos = result.find('\n');
		if (nl_pos != std::string::npos)
		{
			remainder = result.substr(nl_pos + 1, result.size() - nl_pos - 1); // 개행 뒷 부분을 remainder에 저장
			return (result.substr(0, nl_pos)); // 개행 앞 부분을 반환
		}

		int read_byte = recv(socket_fd, buffer, BUF_SIZE, 0);
		if (read_byte <= 0) // 다 읽었거나, 실패했다?
		{
			return (result);
		}
		result += std::string(buffer, read_byte);
	}
}