#include "HttpRequestFactory.hpp"
#include "HttpRequestHandler.hpp"
#include <string>
#include <unistd.h>

#define BUF_SIZE 1024

#define BUFFER_FULL 1
#define DISCONNET 2

// 완전하고 유효한 request 객체 하나를 만들어서 반환
// 반환 값이 NULL인 경우, 소켓이 닫혔거나 소켓에 충분한 데이터가 없음을 뜻함
HttpRequest *HttpRequestFactory::create(int socket_fd)
{
	HttpRequest *request = NULL;
	try
	{
		request = HttpRequestHandler::getChunkedRequest(socket_fd);
		if (request != NULL) // chunked 전송 요청인 경우
		{
			parseChunkedRequest(socket_fd, request);
		}
		else // 일반 요청인 경우
		{
			HttpRequestParser::parse(HttpRequestHandler::getBuffer(socket_fd), request);
			removeRequestInBuffer(socket_fd, request);
		}
	}
	catch (const int& e) // 특별한 예외(완전하지 않은 요청, 청크 시작 요청) 처리
	{
		handlingSpecialException(e, socket_fd, request);
	}
	catch (const char *e) // 유효하지 않은 요청의 예외 처리
	{
		removeRequestInBuffer(socket_fd, request);
		delete request;
		throw;
	}
	return (request);
}

void HttpRequestFactory::parseChunkedRequest(int socket_fd, HttpRequest*& request)
{
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);

	size_t nl_pos = buffer.find("\r\n");
	if (nl_pos == std::string::npos)
		throw INCOMPLETE_REQUEST; // 버퍼에 CRLF가 없는 경우 -> 불완전한 요청
	
	int chunk_size = RequestUtility::hexToDecimal(buffer.substr(0, nl_pos));
	if (chunk_size == FAILURE)
		throw SocketCloseException400(); // 청크 크기를 식별할 수 없는 경우 -> 연결 끊기

	if (buffer.size() < nl_pos + chunk_size + 4)
		throw INCOMPLETE_REQUEST; // 청크 크기 만큼의 청크 데이터가 없는 경우 -> 불완전한 요청

	if (buffer.substr(nl_pos + 2 + chunk_size, 2) != "\r\n")
		throw SocketCloseException400(); // 청크 데이터 직후 CRLF가 없는 경우 -> 연결 끊기

	if (chunk_size == 0) // 마지막 청크 요청인 경우
	{
		HttpRequestHandler::removeChunkedRequest(socket_fd);
		HttpRequestHandler::removePartOfBuffer(socket_fd, 0, 5); // 0\r\n\r\n
		return;
	}

	int content_length = RequestUtility::strToPositiveInt(request->getHeader("Content-Length")) + chunk_size;
	if (content_length > 5000) // TODO : nginx 설정에 따라 최대 바디 크기를 변경하기
		throw SocketCloseException400(); // 제한된 바디 크기를 초과하는 경우 -> 연결 끊기
	request->setHeader("Content-Length", RequestUtility::positiveIntToStr(content_length)); // content-length 값 갱신

	// 청크 데이터를 request body에 추가
	std::string chunk_data = buffer.substr(nl_pos + 2, chunk_size);
	request->addRequestBody(chunk_data);
	HttpRequestHandler::removePartOfBuffer(socket_fd, 0, nl_pos + chunk_size + 4);
}

void HttpRequestFactory::handlingSpecialException(const int& e, int socket_fd, HttpRequest*& request)
{
	if (e == INCOMPLETE_REQUEST) // 불완전한 요청
	{
		delete request;
		request = NULL;
	}
	else // 청크 전송 시작 요청(START_CHUNKED_REQUEST)
	{
		HttpRequestHandler::addChunkedRequest(socket_fd, request);
		removeRequestInBuffer(socket_fd, request);
	}
}

void HttpRequestFactory::removeRequestInBuffer(int socket_fd, HttpRequest* request)
{
	if (!request)
		return;
	
	// 헤더 부분까지 삭제
	size_t end_of_headers = HttpRequestHandler::getBuffer(socket_fd).find("\r\n\r\n") + 4;
	HttpRequestHandler::removePartOfBuffer(socket_fd, 0, end_of_headers);

	// POST 요청이고, 본문 크기를 아는 경우 -> 본문까지 삭제
	if (request->getMethod() == "POST" && 
		request->getHeader("Transfer-Encoding") != "chunked" && 
		request->getHeader("Content-Length") != "")
	{
		int content_length = RequestUtility::strToPositiveInt(request->getHeader("Content-Length"));
		if (content_length != -1)
			HttpRequestHandler::removePartOfBuffer(socket_fd, 0, content_length);
	}
}
