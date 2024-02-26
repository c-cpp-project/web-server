#include "HttpRequestHandler.hpp"

#include <fcntl.h>
#include <sys/socket.h>

#include "../ResponseHandler/FrontController.hpp"

std::map<int, std::string>
    HttpRequestHandler::buffers;  // 요청을 읽어오는 소켓, 버퍼
std::map<int, HttpRequest *>
    HttpRequestHandler::chunkeds;  // chunked 수신 중인 소켓, request 객체

#include <unistd.h>

#define NO_CHUNKED_REQUEST 1
#define IN_PROGRESS_CHUNKED_REQUEST 2
#define LAST_CHUNKED_REQUEST 3

HttpRequestHandler::HttpRequestHandler(int _socket_fd,
                                       ServerConfiguration *_server_config)
    : socket_fd(_socket_fd), server_config(_server_config) {}

int HttpRequestHandler::handle(Event *event)
{
	while (true)
	{
		if (RequestAndResponse(event) == FAILURE)
			return (FAILURE);
		if (buffers[socket_fd] == "")
		{
			if (chunkeds.find(socket_fd) != chunkeds.end())
				return (FAILURE); // 버퍼의 내용은  다 처리했지만, chunked 요청 중인 경우 아직 요청을 더 받아야 함!
			break;
		}
	}
	return (SUCCESS);
}

int HttpRequestHandler::RequestAndResponse(Event *event)
{
	try
	{
		// Request Part
		HttpRequest *request = HttpRequestFactory::create(socket_fd, server_config);
		if (request == NULL)
			return (FAILURE);  // 버퍼에 완전한 요청이 없음
		if (ChunkedRequestHandling(request) == IN_PROGRESS_CHUNKED_REQUEST)
			return (SUCCESS);
		// Response Part
		int kqueue_fd = 0;
		FrontController front_controller(socket_fd, server_config, event);
		front_controller.run(*request);
		delete request;
	} 
	catch (const char *e)
	{
		std::cout << e << ": handle\n";
		errorHandling(e, server_config, event);
	}
	catch (const std::exception &e)
	{
		// socket에 대한 close는 yuikim씨 코드에서 #### 유의사항 1
		removeBuffer(socket_fd);
		removeAndDeleteChunkedRequest(socket_fd);
		// TODO : 뭔가 더 해야 할 수도 있음
		std::cout << "socket 닫기: " << e.what() << "\n";
		errorHandling(e.what(), server_config, event);
		throw;
	}
	return (SUCCESS);
}

// 버퍼가 size만큼 차도록 읽는 함수
void HttpRequestHandler::readRequest(int socket_fd, long size)
{
	if (buffers.find(socket_fd) == buffers.end())
		buffers.insert(std::pair<int, std::string>(socket_fd, ""));
	long read_size = size - buffers[socket_fd].size();
	std::cout << "read size: " << read_size << '\n';
	if (read_size <= 0)
		return;
	char *temp_buffer = new char[read_size];
	int read_byte = recv(socket_fd, temp_buffer, read_size, 0);
	if (read_byte == -1) { // recv 시스템 콜 오류
		delete[] temp_buffer;
		throw SocketCloseException500();
	} else if (read_byte == 0) { // 클라이언트와 연결 끊김
		delete[] temp_buffer;
		throw ClientSocketCloseException();
	}
	buffers[socket_fd] += std::string(temp_buffer, read_byte);
	std::cout << "buffer: " << buffers[socket_fd] << "!end\n";
	delete[] temp_buffer;
}

int HttpRequestHandler::ChunkedRequestHandling(HttpRequest *request) {
  if (request->getHeader("Transfer-Encoding") != "chunked")
    return (NO_CHUNKED_REQUEST);

  // chunkeds 맵에 포함된 chunked 요청인 경우 -> 아직 chunked 요청 전체가 끝나지
  // 않았음
  if (chunkeds.find(socket_fd) != chunkeds.end())
    return (IN_PROGRESS_CHUNKED_REQUEST);

  // 마지막 chunked 요청인 경우
  HttpRequestParser::parseRequestParams(request);
  return (LAST_CHUNKED_REQUEST);
}

void HttpRequestHandler::errorHandling(const char *erorr_code, ServerConfiguration *server_config, Event *event) {
  HttpRequest empty;
  HttpResponse response(socket_fd, server_config, event);

  std::cout << "erorr_code : " << erorr_code << "\n";
  response.setStatusCode(erorr_code);
  response.forward(empty);
  std::cout << "errorHandling done\n";
  // response.flush();
}

void HttpRequestHandler::removeBuffer(int socket_fd) {
  std::map<int, std::string>::iterator it = buffers.find(socket_fd);
  if (it != buffers.end()) buffers.erase(it);
}

HttpRequest *HttpRequestHandler::removeChunkedRequest(int socket_fd) {
  std::map<int, HttpRequest *>::iterator it = chunkeds.find(socket_fd);
  if (it == chunkeds.end()) return (NULL);
  HttpRequest *request = it->second;
  chunkeds.erase(it);
  return (request);
}

void HttpRequestHandler::removeAndDeleteChunkedRequest(int socket_fd) {
  HttpRequest *request = removeChunkedRequest(socket_fd);
  delete request;
}

HttpRequest *HttpRequestHandler::getChunkedRequest(int socket_fd) {
  std::map<int, HttpRequest *>::iterator it = chunkeds.find(socket_fd);
  if (it == chunkeds.end()) return (NULL);
  return (it->second);
}

const std::string &HttpRequestHandler::getBuffer(int socket_fd) {
  return (buffers[socket_fd]);
}

void HttpRequestHandler::addChunkedRequest(int socket_fd,
                                           HttpRequest *request) {
  chunkeds[socket_fd] = request;
}

void HttpRequestHandler::removePartOfBuffer(int socket_fd, int start,
                                            int count) {
  buffers[socket_fd].erase(start, count);
}