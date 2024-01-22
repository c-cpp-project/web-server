#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	// 소켓 생성
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		std::cerr << "Error creating socket\n";
		return (-1);
	}

	// 서버 주소 구조체 구조체 설정, 서버에 연결
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(12345);
	if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Error connecting to server\n";
		close(client_socket);
		return (-1);
	}

	std::cout << "Connected to server on port 12345\n";

	// input 파일에서 데이터 읽어오기
	int input_fd = open("test.txt", O_RDONLY);
	if (input_fd == -1) {
		std::cerr << "Error opening input file\n";
		close(client_socket);
		return (-1);
	}
	char buffer[1024];
	int send_byte = read(input_fd, buffer, 1024);
	if (send_byte == -1) {
		std::cerr << "Error reading input file\n";
		close(client_socket);
		return (-1);
	}
	buffer[send_byte] = '\0';
	std::cout << "send buffer:\n" << buffer << '\n';

	send(client_socket, buffer, send_byte, 0);

	return (0);
}