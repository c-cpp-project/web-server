
#include"Controller.hpp"

#ifndef DEFAULT_CONTROLLER_HPP
# define DEFAULT_CONTROLLER_HPP
class HttpResponse;
class HttpRequest;
class DefaultController : public Controller
{
private:
public:
	private:
public:
	void    service(HttpRequest &request, HttpResponse &response, ServerConfiguration serverConfig);
	DefaultController();
	DefaultController(int masking);
	~DefaultController();
};
#endif