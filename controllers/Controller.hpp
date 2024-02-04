
#include"../ResponseHandler/HttpResponse.hpp"
#include"../HttpRequest/HttpRequest.hpp"
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

#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP

class   HttpRequest;
class   HttpResponse;
class Controller
{
private:
public:
	void    service(HttpRequest &request, HttpResponse &response);
	virtual void    doGet(HttpRequest &request, HttpResponse &response);
	virtual void	doPost(HttpRequest &request, HttpResponse &response);
	virtual	void	doDelete(HttpRequest &request, HttpResponse &response);
	void	   		response200(std::string body, HttpResponse &response);
	virtual ~Controller();
	Controller();
};

#endif
// 자식들이 부모의 service를 통해 자신들의 메서드를 사용한다.

