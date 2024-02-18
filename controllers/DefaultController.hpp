#ifndef DEFAULT_CONTROLLER_HPP
# define DEFAULT_CONTROLLER_HPP
#include"Controller.hpp"

class HttpResponse;
class HttpRequest;
class DefaultController : public Controller
{
private:
public:
	private:
public:
	void    service(HttpRequest &request, HttpResponse &response);
	DefaultController();
	DefaultController(int masking);
	~DefaultController();
};
#endif