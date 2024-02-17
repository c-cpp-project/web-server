#include "HttpRequestHandler.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <string>

void leaks()
{
	system("leaks server");
}

int main()
{
	/*
	// 소켓 생성
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Error create socket\n";
		return (-1);
	}

	// 소켓 구조체 생성, 소켓과 주소를 바인딩
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(12345);
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

	// 클라이언트 연결 대기
	int client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1) {
		std::cerr << "Error accepting client connection\n";
		close(server_socket);
		return (-1);
	}

	std::cout << "Client connected!\n";
	HttpRequestHandler::handle(client_socket);

	close(server_socket);
	close(client_socket);
	*/

	// atexit(leaks);
	// HttpRequestHandler::handle(0);
	return (0);
}