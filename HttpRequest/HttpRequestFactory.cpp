#include "HttpRequestFactory.hpp"
#include "HttpRequestHandler.hpp"
#include <string>
#include <unistd.h>
#include "../server/WebServer.hpp"

// 완전하고 유효한 request 객체 하나를 만들어서 반환
HttpRequest *HttpRequestFactory::create(int socket_fd, ServerConfiguration *&server_config)
{
	HttpRequest *request = NULL;
	try
	{
		request = HttpRequestHandler::getChunkedRequest(socket_fd);
		if (request != NULL) {	// chunked 요청인 경우
			long chunked_request_size = parseChunkedRequest(request, server_config, HttpRequestHandler::getBuffer(socket_fd));
			HttpRequestHandler::removePartOfBuffer(socket_fd, 0, chunked_request_size);
		} else {				// 일반 요청인 경우
			HttpRequestParser::parse(request, server_config, HttpRequestHandler::getBuffer(socket_fd));
			removeRequestInBuffer(socket_fd, request);
		}
	}
	catch (const int& e) // 특별한 예외(완전하지 않은 요청, 청크 시작 및 끝 요청) 처리
	{
		SpecialExceptionHandling(e, socket_fd, request);
	}
	catch (const char *e) // 유효하지 않은 요청의 예외 처리
	{
		removeRequestInBuffer(socket_fd, request);
		delete request;
		throw;
	}
	return (request); // NULL인 경우, 버퍼에 파싱하기에 충분한 데이터가 없음을 뜻함
}

long HttpRequestFactory::parseChunkedRequest(HttpRequest* request, ServerConfiguration *&server_config, const std::string& buffer)
{
	std::vector<std::string> tockens = RequestUtility::splitString(request->getHeader("Host"), ':');
	int port = server_config->getPort();
	ServerConfiguration* temp = WebServer::serverConfigs[std::make_pair(tockens[0], server_config->getPort())];
	if (temp != NULL)
		server_config = temp;

	long client_body_size = server_config->getClientBodySize(request->getPath());

	// 청크 크기가 들어왔는지 확인하기
	size_t end_of_chunk_size = buffer.find("\r\n");
	int body_size_length = RequestUtility::countDigits(client_body_size); // client body size의 자릿 수
	if (end_of_chunk_size == std::string::npos)
	{
		if (buffer.size() >= body_size_length + 2)	// "client body size\r\n" 까지 충분히 읽었다면
			throw SocketCloseException413();		// -> 너무 긴 청크 크기
		else										// "client body size\r\n" 까지 충분히 읽지 못했다면
			throw INCOMPLETE_REQUEST;				// -> 불완전한 청크 크기
	}

	// 청크 크기 파싱하기
	long chunk_size = RequestUtility::hexToDecimal(buffer.substr(0, end_of_chunk_size));
	if (chunk_size == FAILURE)
		throw SocketCloseException400(); // 청크 크기를 식별할 수 없는 경우
	if (chunk_size > client_body_size)
		throw SocketCloseException413(); // 제한된 바디 크기를 초과하는 청크 크기

	// 청크 데이터가 들어왔는지 확인하기
	if (buffer.size() < end_of_chunk_size + chunk_size + 4)
		throw INCOMPLETE_REQUEST; // 버퍼에 청크 크기 만큼의 청크 데이터가 없는 경우
	if (buffer.substr(end_of_chunk_size + 2 + chunk_size, 2) != "\r\n")
		throw SocketCloseException400(); // 청크 데이터 직후 CRLF가 없는 경우

	if (chunk_size == 0) // 마지막 청크 요청인 경우
		throw LAST_CHUNKED_REQUEST;

	// 총 청크 데이터 크기 검사하기 & 청크 데이터를 요청 본문에 추가하기
	long content_length = RequestUtility::strToPositiveLong(request->getHeader("Content-Length")) + chunk_size;
	if (content_length > client_body_size)
		throw SocketCloseException413(); // 제한된 바디 크기를 초과하는 청크 데이터
	request->setHeader("Content-Length", RequestUtility::positiveLongToStr(content_length));
	std::string chunk_data = buffer.substr(end_of_chunk_size + 2, chunk_size);
	request->addRequestBody(chunk_data);

	return (end_of_chunk_size + chunk_size + 4); // 파싱한 청크 요청의 크기 반환
}

void HttpRequestFactory::SpecialExceptionHandling(const int& e, int socket_fd, HttpRequest*& request)
{
	switch(e)
	{
	case INCOMPLETE_REQUEST: // 불완전한 요청
		if (!HttpRequestHandler::getChunkedRequest(socket_fd))
			delete request; // chunk 요청의 경우 request 객체를 삭제하지 않기
		request = NULL;
		break;
	case START_CHUNKED_REQUEST: // 청크 전송 시작 요청
		HttpRequestHandler::addChunkedRequest(socket_fd, request); // chunkeds 맵에 request 객체 추가
		removeRequestInBuffer(socket_fd, request);
		break;
	case LAST_CHUNKED_REQUEST: // 청크 전송 끝 요청
		HttpRequestHandler::removeChunkedRequest(socket_fd); // chunked 맵에서 request 객체 삭제
		HttpRequestHandler::removePartOfBuffer(socket_fd, 0, 5); // 5는 "0\r\n\r\n"의 길이
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
