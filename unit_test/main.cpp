#include"../controllers/Controller.hpp" 
#include"../controllers/ControllerMapping.hpp"
#include"../controllers/MyController.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"../ResponseHandler/ErrorResponse.hpp"
#include"../ResponseHandler/FrontController.hpp"
#include"../ResponseHandler/HttpResponse.hpp"
#include"../HttpConfig.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
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
	int clientSocket = accept(server_socket, NULL, NULL);
	if (clientSocket == -1) {
		std::cerr << "Error accepting client connection\n";
		close(server_socket);
		return (-1);
	}

	std::cout << "Client connected!\n";

	HttpRequest request(clientSocket);

	std::cout << "--- request line ---\n";
	std::cout << "method: " << request.getMethod() << '\n';
	std::cout << "path: " << request.getPath() << '\n';
	std::cout << "query_string: " << request.getQueryString() << '\n';

	std::cout << "\n--- request headers ---\n";
	std::map<std::string, std::string>::iterator iter;
	for(iter = request.getHeadersBegin(); iter != request.getHeadersEnd(); iter++)
		std::cout << iter->first << ": " << iter->second << '\n';

	std::cout << "\n--- request params ---\n";
	for(iter = request.getParamsBegin(); iter != request.getParamsEnd(); iter++)
		std::cout << iter->first << ": " << iter->second << '\n';

	close(server_socket);
	close(clientSocket);

	return (0);
}