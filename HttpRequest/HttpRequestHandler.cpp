#include "HttpRequestHandler.hpp"
#include "SocketCloseException.hpp"
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

// TODO : chunked 수신 시작 요청에서 content-length를 0으로 세팅해놓기
// TODO : 본문의 길이가 너무 긴 것과 아직 소켓에 데이터가 안 들어온 상황을 구분해야 한다.

// run 함수는 무조건 완전하고 정상적인 request 객체 하나만 만들어서 뱉는다.
HttpRequest *HttpRequestHandler::run(int socket_fd)
{
	try
	{
		while (true)
		{
			HttpRequest *request = parseRequestInBuffer(socket_fd);
			if (request != NULL)
				return (request);
			if (buffers[socket_fd].size() == 8000) // TODO : nginx 설정에 따라 최대 헤더 + 바디 크기로 설정하기
				throw SocketCloseException400(); // 제한된 크기를 초과하는 요청 -> 연결 끊기
			if (readRequest(socket_fd) == FAILURE) // 소켓에 읽을 데이터가 없음
				return (NULL);
		}
	}
	catch (const std::exception& e)
	{
		removeInBuffers(socket_fd);
		removeInChunkeds(socket_fd);
		throw;
	}
}

HttpRequest *HttpRequestHandler::parseRequestInBuffer(int socket_fd)
{
	HttpRequest *request = NULL;
	try
	{
		request = getRequestInChunkeds(socket_fd);
		if (request != NULL) // chunked 수신 중인 소켓인 경우
			return (parseChunkedRequest(socket_fd, request));
		return (HttpRequestParser::parse(buffers[socket_fd], request));
	}
	catch (int status)
	{
		delete request;
		switch(status)
		{
			case INCOMPLETE_REQUEST: // 요청을 더 읽어오자
				return (NULL);
			case START_CHUNKED_REQUEST: // 청크 맵에 요청을 저장하고, 더 읽어오자
				chunkeds[socket_fd] = request;
				buffers[socket_fd].erase(0,  buffers[socket_fd].find("\r\n\r\n") + 3);
				return (NULL);
			case BAD_REQUEST: // 현재 요청을 거부하자
				// TODO : 지금은 무조건 연속된 CRLF까지 제거함. 본문이 있는 경우엔 본문까지 제거해야 함!
				buffers[socket_fd].erase(0, buffers[socket_fd].find("\r\n\r\n") + 3);
				throw RequestUtility::positiveIntToStr(status);
		}
	}

	return (request);
}

int HttpRequestHandler::readRequest(int socket_fd)
{
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) // 소켓을 논블로킹 모드로 설정
		throw SocketCloseException500();
	
	int remaining_size = 8000 - buffers[socket_fd].size(); // 버퍼 여유 크기 / TODO : nginx 설정에 따라 최대 헤더 + 바디 크기로 설정하기
	char *temp_buffer = new char[remaining_size];
	int read_byte = recv(socket_fd, temp_buffer, remaining_size, 0);
	if (read_byte == -1) // 소켓에 읽을 데이터가 없음
		return (FAILURE);
	if (read_byte == 0) // 클라이언트와 연결이 끊김 / TODO : Kqueue 쪽에 (더이상 이 fd를 감시하지 말라고) 알릴 필요는 없을까?
		throw SocketCloseException400(); // TODO : 오류 응답을 안 보내는 예외 클래스도 필요하겠네
	buffers[socket_fd] += std::string(temp_buffer, read_byte);
}

HttpRequest *HttpRequestHandler::getRequestInChunkeds(int socket_fd)
{
	std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
	if (it == chunkeds.end())
		return (NULL);
	return (it->second);
}

HttpRequest *HttpRequestHandler::parseChunkedRequest(int socket_fd, HttpRequest *request)
{
	size_t nl_pos = buffers[socket_fd].find("\r\n");
	if (nl_pos == std::string::npos)
		throw INCOMPLETE_REQUEST; // 버퍼에 CRLF가 없는 경우 -> 불완전한 요청
	
	int chunk_size = RequestUtility::hexToDecimal(buffers[socket_fd].substr(0, nl_pos)); // 청크 크기 파싱
	if (chunk_size == FAILURE || chunk_size > 1000) // TODO : nginx 설정에 따라 최대 바디 크기를 변경하기
		throw SocketCloseException400(); // 청크 크기를 식별할 수 없거나, 제한된 크기를 초과하는 경우 -> 연결 끊기

	// content-length 헤더 값 갱신
	int content_length = RequestUtility::strToPositiveInt(request->getHeader("Content-Length"));
	request->setHeader("Content-Length", RequestUtility::positiveIntToStr(content_length + chunk_size));

	if (buffers[socket_fd].size() < nl_pos + chunk_size + 4)
		throw INCOMPLETE_REQUEST; // 청크 크기 만큼의 청크 데이터가 없는 경우 -> 불완전한 요청

	if (buffers[socket_fd].substr(nl_pos + 2 + chunk_size, 2) == "\r\n")
		throw SocketCloseException400(); // 청크 데이터 직후 CRLF가 없는 경우 -> 연결 끊기

	if (chunk_size == 0) // 마지막 청크 요청인 경우 -> request 객체 반환
		return (request);

	// 청크 데이터를 request body에 추가
	std::string chunk_data = buffers[socket_fd].substr(nl_pos + 2, chunk_size);
	request->addRequestBody(chunk_data);
	throw INCOMPLETE_REQUEST;
}

void HttpRequestHandler::removeInBuffers(int socket_fd)
{
	std::map<int, std::string>::iterator it = buffers.find(socket_fd);
	if (it != buffers.end())
		buffers.erase(it);
}

void HttpRequestHandler::removeInChunkeds(int socket_fd)
{
	std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
	if (it != chunkeds.end())
	{
		delete it->second;
		chunkeds.erase(it);
	}
}
