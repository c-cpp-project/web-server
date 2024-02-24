
#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP
#include<exception>
#include<iostream>
#include"../ResponseConfig.hpp"
#include"../controllers/Controller.hpp"
#include"../controllers/ControllerMapping.hpp"
#include"../server/ServerConfiguration.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"HttpResponse.hpp"
#include<string>
#include<sys/fcntl.h>
#include"MultiRequest.hpp"
#include"../server/Event.hpp"

class HttpHandler;
class HttpRequest;
class HttpResponse;
class Event;
class FrontController
{
private:
	int 				socketfd;
	ServerConfiguration	*serverConfig;
	Event				*event;
public:
	FrontController(int socketfd);
	FrontController(int socketfd, int fd);
	FrontController(int socketfd, ServerConfiguration *serverConfig, Event *event);
	FrontController();
	~FrontController();

	void    run(HttpRequest tmp);
};
#endif

void	sigchldHandler(int signo);
void	alarmHandler(int signo);

// Http의 url을 보고 Location에 포함된다면 등록된 Location에 대응되는 경로를 얻어서 허용된 메서드를 실행한다.
// 만약에 Location에 포함되지 않는다면 해당 경로를 그대로 사용한다.