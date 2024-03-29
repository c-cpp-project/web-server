
#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP
#include"../ResponseHandler/HttpResponse.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"../server/setting/ConfigParser.hpp"
#include"../server/ServerConfiguration.hpp"
#include "ChildProcess.hpp"
#include"../Bean/HttpHandler.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>

class   HttpRequest;
class   HttpResponse;
class	HttpHandler;
class Controller
{
private:
	int 				masking; // 허용 메서드 7 -> 모두 허용
protected:
	std::string			mLocation;
public:
	virtual void    service(HttpRequest &request, HttpResponse &response) = 0;
	void    doGet(HttpRequest &request, HttpResponse &response);
	void	doPost(HttpRequest &request, HttpResponse &response);
	void	doDelete(HttpRequest &request, HttpResponse &response);
	void	response200(std::string body, HttpResponse &response);
	bool	isAcceptableMethod(std::string method);
	std::pair<std::string, std::string>	getFileName(HttpRequest &request);
	virtual ~Controller();
	Controller();
	Controller(int masking);
	Controller(int masking, std::string mLocation);
	Controller(int masking, ServerConfiguration	*serverConfig, std::string mLocation);

	char		**envpList(HttpRequest &request);
	void 		freeEnvpList(char *const *envp);
	std::string	changeToUnderbar(std::string src);
	// 모듈화
	void			classifyEvent(HttpRequest &request, HttpResponse &response, const char *cgi_python);
	void			readEventRegsiter(int readfd[2], HttpResponse &response, size_t bodySize);
	void			writeEventRegister(int writefd[2], int readfd[2], HttpResponse &response, std::string data);
};

#endif
// 자식들이 부모의 service를 통해 자신들의 메서드를 사용한다.
// 구조를 바꿔야 한다.
