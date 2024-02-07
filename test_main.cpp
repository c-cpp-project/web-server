#include"HttpRequest/HttpHeaders.hpp"
#include"HttpRequest/HttpRequest.hpp"
#include"HttpRequest/HttpRequestHandler.hpp"
#include"HttpRequest/HttpRequestParser.hpp"
#include"HttpRequest/RequestLine.hpp"
#include"HttpRequest/RequestParams.hpp"
#include"HttpRequest/RequestUtility.hpp"
#include"HttpRequest/SocketCloseException.hpp"
#include"controllers/Controller.hpp"
#include"controllers/ControllerMapping.hpp"
#include"controllers/MyController.hpp"
#include"ResponseHandler/FrontController.hpp"
#include"ResponseHandler/HttpResponse.hpp"
#include"ResponseHandler/InterruptApi.hpp"
#include"ResponseHandler/MultiRequest.hpp"
#include"ResponseConfig.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int	tcp_listening(int &server_socket)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Error create socket\n";
		return (-1);
	}

	// 소켓 구조체 생성, 소켓과 주소를 바인딩
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(8080);
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Error binding socket\n";
		close(server_socket);
		return (-1);
	}

	// 연결 대기열 설정 
	if (listen(server_socket, 5) == -1) {
		std::cerr << "Error setting up listen queue\n";
		close(server_socket);
		return (-1);
	}

	std::cout << "Server is listening on port 12345...\n";
	return (1);
}

int	tcp_connection(int &clientSocket, int &server_socket)
{
	// 클라이언트 연결 대기
	clientSocket = accept(server_socket, NULL, NULL);
	if (clientSocket == -1) {
		std::cerr << "Error accepting client connection\n";
		close(server_socket);
		return (-1);
	}
	std::cout << "Client connected!\n";
	return (clientSocket);
}

int	endpointTest(void)
{
	// 소켓 생성
	ResponseConfig	ResponseConfig;
	int	clientSocket;
	int	server_socket;

	tcp_listening(server_socket);
	clientSocket = tcp_connection(clientSocket, server_socket);
	if (clientSocket== -1)
		return (-1);
	// FrontController frontController(clientSocket);

	Server server;
	ServerConfiguration server_info(server);
	HttpRequestHandler httpRequestHandler(clientSocket, server_info);
	while (true)
	{
		httpRequestHandler.handle();
	}
	// HttpRequestHandler::handle(clientSocket); // #### 유의사항 2
	close(clientSocket);
	close(server_socket);
	return (0);
}

int	requestToResponseTest(void)
{
	HttpConfig httpConfig;
	Server server;
	ServerConfiguration server_info(server);
	HttpRequestHandler httpRequestHandler(1, server_info);
	httpRequestHandler.handle();
	return (0);
}

int main()
{
	// requestToResponseTest();
	endpointTest();
	return (0);
}