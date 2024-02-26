#include "HttpRequestFactory.hpp"
#include "HttpRequestHandler.hpp"
#include <string>
#include <unistd.h>

#define BUF_SIZE 1024

#define BUFFER_FULL 1
#define DISCONNET 2

// 완전하고 유효한 request 객체 하나를 만들어서 반환
// 반환 값이 NULL인 경우, 소켓이 닫혔거나 소켓에 충분한 데이터가 없음을 뜻함
HttpRequest *HttpRequestFactory::create(int socket_fd, ServerConfiguration *server_config)
{
	HttpRequest *request = NULL;
	try
	{ 
		request = HttpRequestHandler::getChunkedRequest(socket_fd);
		if (request != NULL) // chunked 전송 요청인 경우
		{
			readChunkedRequest(socket_fd, request, server_config);
			parseChunkedRequest(socket_fd, request, server_config);
		}
		else // 일반 요청인 경우
		{
			HttpRequestParser::parse(socket_fd, request, server_config);
			removeRequestInBuffer(socket_fd, request);
		}
	}
	catch (const int& e) // 특별한 예외(완전하지 않은 요청, 청크 시작 요청) 처리
	{
		SpecialExceptionHandling(e, socket_fd, request);
	}
	catch (const char *e) // 유효하지 않은 요청의 예외 처리
	{
		removeRequestInBuffer(socket_fd, request);
		delete request;
		throw;
	}
	return (request);
}

// TODO : 청크 크기랑 데이터 읽는 부분 나누면 좋겠다.
long HttpRequestFactory::readChunkedRequest(int socket_fd, HttpRequest* request, ServerConfiguration *server_config)
{
	// "청크크기\r\n청크데이터\r\n" 형식에서
	// 청크데이터를 제외한 부분을 고려하기 위해 23(long 최대 자릿 수 + 두 개의 CRLF) 추가
	long client_body_size = server_config->getClientBodySize(request->getPath()) + 23;
	HttpRequestHandler::readRequest(socket_fd, client_body_size);
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);

	size_t end_of_chunk_size = buffer.find("\r\n");
	if (end_of_chunk_size == std::string::npos)
	{
		if (buffer.size() >= 21) // "long최대\r\n" 까지 충분히 읽었다면
			throw SocketCloseException400();
		throw INCOMPLETE_REQUEST;
	}
	return (end_of_chunk_size);
}

void HttpRequestFactory::parseChunkedRequest(int socket_fd, HttpRequest* request, ServerConfiguration *server_config)
{
	long end_of_chunk_size = readChunkedRequest(socket_fd, request, server_config);
	const std::string& buffer = HttpRequestHandler::getBuffer(socket_fd);
	
	int chunk_size = RequestUtility::hexToDecimal(buffer.substr(0, end_of_chunk_size));
	if (chunk_size == FAILURE)
		throw SocketCloseException400(); // 청크 크기를 식별할 수 없는 경우 -> 연결 끊기
	if (chunk_size > server_config->getClientBodySize(request->getPath()))
		throw SocketCloseException400(); // 제한된 바디 크기를 초과하는 청크 데이터
	if (buffer.size() < end_of_chunk_size + chunk_size + 4)
		throw INCOMPLETE_REQUEST; // 청크 크기 만큼의 청크 데이터가 없는 경우 -> 불완전한 요청
	if (buffer.substr(end_of_chunk_size + 2 + chunk_size, 2) != "\r\n")
		throw SocketCloseException400(); // 청크 데이터 직후 CRLF가 없는 경우 -> 연결 끊기

	if (chunk_size == 0) // 마지막 청크 요청인 경우
		throw LAST_CHUNKED_REQUEST;

	int content_length = RequestUtility::strToPositiveLong(request->getHeader("Content-Length")) + chunk_size;
	if (content_length > server_config->getClientBodySize(request->getPath()))
		throw SocketCloseException400(); // 제한된 바디 크기를 초과하는 청크 데이터
	request->setHeader("Content-Length", RequestUtility::positiveLongToStr(content_length)); // content-length 값 갱신

	// 청크 데이터를 request body에 추가
	std::string chunk_data = buffer.substr(end_of_chunk_size + 2, chunk_size);
	request->addRequestBody(chunk_data);
	HttpRequestHandler::removePartOfBuffer(socket_fd, 0, end_of_chunk_size + chunk_size + 4);
}

void HttpRequestFactory::SpecialExceptionHandling(const int& e, int socket_fd, HttpRequest*& request)
{
	switch(e)
	{
	case INCOMPLETE_REQUEST: // 불완전한 요청
		delete request;
		request = NULL;
		break;
	case START_CHUNKED_REQUEST: // 청크 전송 시작 요청
		HttpRequestHandler::addChunkedRequest(socket_fd, request);
		removeRequestInBuffer(socket_fd, request);
		break;
	case LAST_CHUNKED_REQUEST: // 청크 전송 끝 요청
		HttpRequestHandler::removeChunkedRequest(socket_fd);
		HttpRequestHandler::removePartOfBuffer(socket_fd, 0, 5); // 0\r\n\r\n
		break;
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
		long content_length = RequestUtility::strToPositiveLong(request->getHeader("Content-Length"));
		if (content_length != FAILURE)
			HttpRequestHandler::removePartOfBuffer(socket_fd, 0, content_length);
	}
}
