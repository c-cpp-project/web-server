
#include<exception>
#include<iostream>
#include"../ResponseConfig.hpp"
#include"../controllers/Controller.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"HttpResponse.hpp"
#include<string>
#include<sys/fcntl.h>
#include"MultiRequest.hpp"
#include"../Bean/HttpHandler.hpp"
#include"../server/Event.hpp"

#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

class HttpHandler;
class HttpRequest;
class HttpResponse;
class FrontController
{
private:
	int 				socketfd;
	ServerConfiguration	*serverConfig;
	Event				*event;
public:
	FrontController(int socketfd);
	FrontController(ServerConfiguration *serverConfig, Event *event);
	FrontController();
	~FrontController();

	void    run(HttpRequest tmp);
};
#endif


// Http의 url을 보고 Location에 포함된다면 등록된 Location에 대응되는 경로를 얻어서 허용된 메서드를 실행한다.
// 만약에 Location에 포함되지 않는다면 해당 경로를 그대로 사용한다.