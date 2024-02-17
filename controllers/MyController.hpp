
#include"Controller.hpp"

#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP
class HttpResponse;
class HttpRequest;
class ServerConfiguration;
class Event;
class MyController : public Controller
{
private:
public:
	void	service(HttpRequest &request, HttpResponse &response);
	MyController();
	MyController(int masking);
	MyController(int masking, ServerConfiguration* serverConfig, Event *event);
	~MyController();
};
#endif