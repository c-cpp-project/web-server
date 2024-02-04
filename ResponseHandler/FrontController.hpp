
#include<exception>
#include<iostream>
#include"../HttpConfig.hpp"
#include"../controllers/Controller.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"HttpResponse.hpp"
#include<string>
#include<sys/fcntl.h>
#include"MultiRequest.hpp"

#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

class HttpRequest;
class HttpResponse;
class FrontController
{
private:
	int socketfd;
	int kqueuefd;
	HttpRequest tmp;
public:
	FrontController(int socketfd);
	FrontController(int kqueuefd, int socketfd, HttpRequest &request);
	FrontController();
	~FrontController();

	void    run();
};
#endif