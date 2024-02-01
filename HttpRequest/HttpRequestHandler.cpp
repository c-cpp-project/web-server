#include "HttpRequestHandler.hpp"
#include "SocketCloseException.hpp"
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

#define SUCCEEDED 0
#define DISCONNECTED 1

void HttpRequestHandler::run(int socket_fd)
{
	if (readAll(socket_fd) == DISCONNECTED)
	{
		std::map<int, std::string>::iterator iter = buffers.find(socket_fd);
		if (iter != buffers.end())
			buffers.erase(iter);
		return;
	}

	while (true)
	{
		if (buffers[socket_fd] == "")
			break;
		
		HttpRequest *request;
		size_t end_of_headers;
		try
		{
			// TODO : 마지막 chunked 메시지라면 request body에 0 넣기
			std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
			if (it != chunkeds.end())
			{
				receiveChunk(buffers[socket_fd], it->second);
				// ...
				return;
			}

			// TODO : 버퍼를 최대 헤더 크기만큼 동적으로 할당하고, 여기서 WRONG_REQUEST를 던지자
			// 버퍼에 CRLF 연속 2개 없다면(요청 헤더까지 안 들어왔다면) -> 불완전한 메시지
			end_of_headers = buffers[socket_fd].find("\r\n\r\n");
			if (end_of_headers == std::string::npos)
				throw INCOMPLETE_REQUEST;
			
			request = HttpRequestParser::parse(buffers[socket_fd]); // 요청 메시지 파싱
		}
		catch (int err_code)
		{
			switch (err_code)
			{
				case INCOMPLETE_REQUEST: processIncompleteRequest(request);
					return;
				case START_CHUNKED_REQUEST: processStartChunkedRequest(socket_fd, request);
					return;
				case WRONG_CHUNKED_REQUEST: processWrongChunkedRequest(socket_fd);
					return;
				case WRONG_REQUEST_UNKNOWN_RANGE: processWrongRequestUnknownRange(socket_fd, request);
					return;
				case WRONG_REQUEST_KNOWN_RANGE: processWrongRequestKnownRange(socket_fd, request);
					break;
			}
		}
		
	}
}

void HttpRequestHandler::receiveChunk(const std::string& buffer, HttpRequest *request)
{
	// 버퍼에 CRLF가 없는 경우 -> 연결 끊기
	size_t nl_pos = buffer.find("\r\n");
	if (nl_pos == std::string::npos)
	{
		// TODO : 400 응답 보내고, 연결 끊고, buffers와 chunkeds 삭제하기
		// 그러려면 연결을 끊고 싶다는 의지를 response 쪽에 전달해야 하는데! 400 응답을 보내고 끊어야 하니까.
		throw "400";
	}

	// 첫 CRLF 전까지의 (청크 크기를 나타내는 16진수) 문자열 -> 10진수 숫자로 변환
	int chunk_size = convertHexToDecimal(buffer.substr(0, nl_pos));
	if (chunk_size == -1)
	{
		// TODO : 400 응답 보내고, 연결 끊고, buffers와 chunkeds 삭제하기
		throw "400";
	}

	// 버퍼에 청크 크기만큼의 청크 데이터가 없는 경우 -> 불완전한 메시지
	if (buffer.size() < nl_pos + chunk_size + 4)
		return;

	// 청크 데이터 직후 CRLF가 없는 경우 -> 연결 끊기
	if (buffer[nl_pos + chunk_size + 2] != '\r' || buffer[nl_pos + chunk_size + 3] != '\n')
	{
		// TODO : 400 응답 보내고, 연결 끊고, buffers와 chunkeds 삭제하기
		throw "400";
	}

	if (request->getHeader("CONTENT-TYPE") == "multipart/form-data")
	{
		// TODO : 파싱, 하위 헤더를 header에 추가, 순수 본문을 body에 대입
	}
	else
	{
		request->setRequestBody(buffer.substr(nl_pos + 2, chunk_size));
	}
}

// socket_fd로부터 모든 데이터를 읽어와서 버퍼에 저장하기
int HttpRequestHandler::readAll(int socket_fd)
{
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) // 소켓을 논블로킹 모드로 설정
		throw "500"; // Internal Server Error

	char temp_buffer[BUF_SIZE];
	while (true)
	{
		int read_byte = recv(socket_fd, temp_buffer, BUF_SIZE, 0);

		// 더이상 버퍼에 읽을 데이터가 없다고 판단
		// TODO : recv에서 오류가 발생한 거면 어쩌지, errno를 확인할 수 없으니 구분이 불가능함
		if (read_byte == -1)
			return (SUCCEEDED);
		
		// 클라이언트와 연결이 끊김
		// TODO : kqueue 쪽에 (더이상 이 fd를 감시하지 말라고) 알릴 필요는 없을까?
		if (read_byte == 0)
			return (DISCONNECTED);

		buffers[socket_fd] += std::string(temp_buffer, read_byte);
	}
}

int HttpRequestHandler::convertHexToDecimal(const std::string &hex_string)
{
	int result = 0;
	for(size_t i = 0; i < hex_string.size(); i++)
	{
		int char_to_decimal;
		if ('0' <= hex_string[i] && hex_string[i] <= '9')
			char_to_decimal = hex_string[i] - '0';
		else if ('A' <= hex_string[i] && hex_string[i] <= 'F')
			char_to_decimal = hex_string[i] - 'A' + 10;
		else if ('a' <= hex_string[i] && hex_string[i] <= 'f')
			char_to_decimal = hex_string[i] - 'a' + 10;
		else
			return (-1);
		result = result * 16 + char_to_decimal;
	}
	return (result);
}

// 불완전한 요청 처리
void HttpRequestHandler::processIncompleteRequest(HttpRequest *request)
{
	delete request; // 1. request 삭제
}

// chunked 수신을 시작하는 post 요청 처리
void HttpRequestHandler::processStartChunkedRequest(int socket_fd, HttpRequest *request)
{
	chunkeds[socket_fd] = request; // 1. request를 chunked에 추가
	size_t end_of_headers = buffers[socket_fd].find("\r\n\r\n") + 3;
	buffers[socket_fd].erase(0, end_of_headers + 3); // 2. 버퍼에서 해당 메시지만 삭제
}

// 잘못된 chunked 요청 처리
void HttpRequestHandler::processWrongChunkedRequest(int socket_fd)
{
	std::map<int, HttpRequest*>::iterator chunkeds_it = chunkeds.find(socket_fd);
	if (chunkeds_it != chunkeds.end()) {
		delete chunkeds_it->second; // 1. chunkeds에서 request 삭제
		chunkeds.erase(chunkeds_it);
	}
	std::map<int, std::string>::iterator buffers_it = buffers.find(socket_fd);
	if (buffers_it != buffers.end())
		buffers.erase(buffers_it); // 2. buffer 삭제
	throw SocketCloseException(); // 3. 클라이언트와 연결 끊기
}

// 잘못된 일반 요청 처리 (현재 메시지 범위를 모름)
void HttpRequestHandler::processWrongRequestUnknownRange(int socket_fd, HttpRequest *request)
{
	delete request; // 1. request 객체 삭제
	std::map<int, std::string>::iterator buffers_it = buffers.find(socket_fd);
	if (buffers_it != buffers.end())
		buffers.erase(buffers_it); // 2. buffer 삭제
	throw "400"; // 3. 400 응답
}

// 잘못된 일반 요청 처리 (현재 메시지 범위를 알고 있음)
// 현재 메시지 범위를 모르는 일반 요청 파싱 중 예외가 발생한 경우 -> request 객체 삭제, 해당 요청까지만 버퍼에서 비우기, 400 응답
void HttpRequestHandler::processWrongRequestKnownRange(int socket_fd, HttpRequest *request)
{
	delete request; // 1. request 객체 삭제
	// TODO : 2. 해당 요청까지만 버퍼에서 비우기
	throw "400"; // 3. 400 응답
}

// TODO : 소켓에 여러 메시지가 쌓여 있고, 첫 메시지에서 400 응답을 보내더라도
// 연결을 끊지 않았다면, 그 다음 메시지를 이어서 처리할 수 있는 로직이 필요하다.
// requestHandler 안에서 response를 호출하는 것과
// requestHandler 에서 던진 예외를 respone 쪽에서 잡는다는 두 개념이 충돌한 것 같다.
