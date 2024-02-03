#include "HttpRequestHandler.hpp"
#include "HttpRequestFactory.hpp"
#include <fcntl.h>
#include <sys/socket.h>

// #include "../ResponseHandler/HttpResponse.hpp"
// #include "../ResponseHandler/FrontController.hpp"

std::map<int, std::string> HttpRequestHandler::buffers; // 요청을 읽어오는 소켓, 버퍼
std::map<int, HttpRequest*> HttpRequestHandler::chunkeds; // chunked 수신 중인 소켓, request 객체

// 1. 소켓으로부터 이벤트가 감지되고, HttpRequestHandler.handle()이 호출된다.
// 2. 해당 소켓으로부터 (지정된 버퍼 크기만큼) 데이터를 (소켓과 매핑된) 버퍼에 읽어온다.
// 3. (소켓과 매핑된) 버퍼에 데이터가 남아있는 동안, 아래의 과정을 반복한다.
//     1. HttpRequestFactory의 create 함수를 호출하여 request 객체를 만든다.
//     2. request 객체가 NULL이라면 (버퍼에 있는 내용이 하나의 완전한 요청이 아님 or 연결이 끊김) 반복을 종료하고 handle()을 빠져나간다.
//     3. request 객체가 NULL이 아니라면 frontController.run()을 호출한다.

// TODO : Content-Length가 없는 (본문이 있는) POST 요청의 경우, 처음에 411 응답 후 헤더까지만 지우고 바디가 버퍼에 남아버려서 계속 411 오류를 내보냄
// 그 다음 요청이 들어올 때까지 계속 411 오류가 날텐데 어떡하지

#include <unistd.h>

void HttpRequestHandler::handle(int socket_fd)
{
	try
	{
		// int read_status = readRequest(socket_fd);
		// if (read_status == FAILURE) // 클라이언트와 연결이 끊긴 경우
			// return;
		
		// buffers[socket_fd] = "GET / HTTP/1.1\r\nhost: 1234\r\n\r\n";
		buffers[socket_fd] = "POST /example-endpoint HTTP/1.1\r\nHost: example.com\r\nContent-Length: 388\r\nContent-Type: multipart/form-data; boundary=---------------------------1234567890123456789012345678\r\n\r\n-----------------------------1234567890123456789012345678\r\nContent-Disposition: form-data; name=\"text_field\"\r\n\r\nThis is a simple text field.\r\n-----------------------------1234567890123456789012345678\r\nContent-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\r\nContent-Type: text/plain\r\n\r\nContents of the file go here.\r\n-----------------------------1234567890123456789012345678--";
		// buffers[socket_fd] = "POST /example-endpoint HTTP/1.1\r\nHost: example.com\r\nContent-Type: multipart/form-data; boundary=---------------------------1234567890123456789012345678\r\n\r\n-----------------------------1234567890123456789012345678\r\nContent-Disposition: form-data; name=\"text_field\"\r\n\r\nThis is a simple text field.\r\n-----------------------------1234567890123456789012345678\r\nContent-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\r\nContent-Type: text/plain\r\n\r\nContents of the file go here.\r\n-----------------------------1234567890123456789012345678--";
		
		while (buffers[socket_fd] != "")
		{
			try
			{
				HttpRequest *request = HttpRequestFactory::create(socket_fd);
				if (request == NULL && buffers[socket_fd].size() == 8000) // TODO : nginx 설정에 따라 최대 헤더 + 바디 크기로 설정하기
					throw SocketCloseException400(); // 제한된 크기를 초과하는 요청
				if (request == NULL)
					return; // 버퍼에 완전한 요청이 없거나 연결이 끊김
				
				// TODO : chunked 요청과 chunked 끝 요청을 구분할 수 있는 다른 방법이 없을까?
				// chunked 요청이지만, chunkeds 맵에 포함된 경우 -> 아직 chunked 요청 전체가 끝나지 않았음
				if (request->getHeader("Transfer-Encoding") != "chunked" && chunkeds.find(socket_fd) != chunkeds.end())
					continue;

				std::cout << "----- Request line -----\n";
				std::cout << "method: " << request->getMethod() << '\n';
				std::cout << "path: " << request->getPath() << '\n';
				std::cout << "query string: " << request->getQueryString() << "\n\n";
			
				std::cout << "----- Request Header ------\n";
				std::map<std::string, std::string>::iterator it;
				for(it = request->getHeadersBegin(); it != request->getHeadersEnd(); it++)
					std::cout << it->first << ": " << it->second << "\n";
				std::cout << '\n';

				std::cout << "----- Request Body -----\n";
				std::cout << request->getBody() << "\n\n";

				std::cout << "----- Request Params -----\n";
				for(it = request->getParamsBegin(); it != request->getParamsEnd(); it++)
					std::cout << it->first << ": " << it->second << "\n";
				std::cout << '\n';
				
				// HttpResponse response(socket_fd);
				// FrontController front_controller(*request, response);
				// front_controller.run();

				delete request;
			}
			catch (const char *e)
			{
				std::cout << "Error: " << e << '\n';
			}
		}
	}
	catch (const std::exception& e)
	{
		removeBuffer(socket_fd);
		removeChunkedRequest(socket_fd);

		std::cout << "socket 닫기: " << e.what();

		// HttpRequest request;
		// HttpResponse response(socket_fd);
		// response.setStatusCode(e.what());
		// response.forward(request, response);
		// response.flush();
	}
}

int HttpRequestHandler::readRequest(int socket_fd)
{
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) // 소켓을 논블로킹 모드로 설정
		throw SocketCloseException500();

	int remaining_size = 8000 - buffers[socket_fd].size(); // TODO : nginx 설정에 따라 최대 헤더 + 바디 크기로 설정하기
	char *temp_buffer = new char[remaining_size]; // 여유 크기만큼 버퍼 메모리 할당

	int read_byte = recv(socket_fd, temp_buffer, remaining_size, 0);
	if (read_byte <= 0)
	{
		delete[] temp_buffer;
		if (read_byte == -1)
			throw SocketCloseException500(); // recv 시스템 콜 오류
		return (FAILURE); // 클라이언트와 연결 끊김 // TODO : 클라이언트 측에서 연결을 끊은 경우, 소켓을 닫는 건 kqueue 쪽에서 해주려나?
	}

	buffers[socket_fd] += std::string(temp_buffer, read_byte);
	delete[] temp_buffer;

	return (SUCCESS);
}

void HttpRequestHandler::removeBuffer(int socket_fd)
{
	std::map<int, std::string>::iterator it = buffers.find(socket_fd);
	if (it != buffers.end())
		buffers.erase(it);
}

void HttpRequestHandler::removeChunkedRequest(int socket_fd)
{
	std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
	if (it != chunkeds.end())
	{
		delete it->second;
		chunkeds.erase(it);
	}
}

HttpRequest *HttpRequestHandler::getChunkedRequest(int socket_fd)
{
	std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
	if (it == chunkeds.end())
		return (NULL);
	return (it->second);
}

const std::string& HttpRequestHandler::getBuffer(int socket_fd)
{
	return (buffers[socket_fd]);
}

void HttpRequestHandler::addChunkedRequest(int socket_fd, HttpRequest *request)
{
	chunkeds[socket_fd] = request;
}

void HttpRequestHandler::removePartOfBuffer(int socket_fd, int start, int count)
{
	buffers[socket_fd].erase(start, count);
}