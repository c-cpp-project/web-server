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
		size_t nl_pos = result.find("\r\n"); // CRLF 찾기
		if (nl_pos != std::string::npos) // CRLF가 있다면
		{
			remainder = result.substr(nl_pos + 2, result.size() - nl_pos - 2); // CRLF 찾기 뒷 부분을 remainder에 저장
			return (result.substr(0, nl_pos)); // CRLF 찾기 앞 부분을 반환
		}

		int read_byte = recv(socket_fd, buffer, BUF_SIZE, 0); // CRLF가 없다면 더 읽어오기
		if (read_byte <= 0) // 다 읽었거나, 실패했다?
		{
			remainder = "";
			return (result);
		}
		result += std::string(buffer, read_byte);
	}
}

std::string HttpRequestReader::getBytes(size_t byte)
{
	char buffer[BUF_SIZE];
	std::string result = remainder;

	while (true)
	{
		if (result.size() >= byte)
		{
			remainder = result.substr(byte, result.size() - byte);
			return (result.substr(0, byte));
		}

		int read_byte = recv(socket_fd, buffer, BUF_SIZE, 0);
		if (read_byte <= 0) // 다 읽었거나, 실패했다?
		{
			remainder = "";
			return (result);
		}
		result += std::string(buffer, read_byte);
	}
}

bool HttpRequestReader::readAll()
{
	char buffer[BUF_SIZE];
	int read_byte = recv(socket_fd, buffer, BUF_SIZE, 0);
	if (remainder == "" && read_byte == 0)
		return (true);
	remainder += std::string(buffer, read_byte);
	return (false);
}