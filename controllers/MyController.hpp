
#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP
#include"Controller.hpp"
#include <sys/stat.h>

class HttpResponse;
class HttpRequest;
class ServerConfiguration;
class Event;
class Controller;
class MyController : public Controller
{
public:
	void	service(HttpRequest &request, HttpResponse &response);
	void	runCgiScript(HttpRequest &request, HttpResponse &response);
	void	runService(HttpRequest &request, HttpResponse &response);
	std::string	findFullPath(std::string fullpath, std::string defaultpage);
	std::string	getLocationIndex(ServerConfiguration *serverConfig, std::string path);
	MyController();
	MyController(int masking);
	MyController(int masking, std::string mLocation);
	~MyController();
};
#endif