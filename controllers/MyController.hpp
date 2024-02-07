
#include"Controller.hpp"

#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP
class HttpResponse;
class HttpRequest;
class MyController : public Controller
{
private:
public:
	void    service(HttpRequest &request, HttpResponse &response, ServerConfiguration *serverConfig);
	MyController();
	MyController(int masking);
	~MyController();
};
#endif