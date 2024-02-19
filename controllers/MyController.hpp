
#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP
#include"Controller.hpp"

class HttpResponse;
class HttpRequest;
class ServerConfiguration;
class Event;
class Controller;
class MyController : public Controller
{
public:
	void	service(HttpRequest &request, HttpResponse &response);
	MyController();
	MyController(int masking);
	~MyController();
};
#endif