
#include"Controller.hpp"

#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP
class HttpResponse;
class HttpRequest;
class MyController : public Controller
{
private:
public:
	void    doGet(HttpRequest &request, HttpResponse &response);
	void	doPost(HttpRequest &request, HttpResponse &response);
	void	doDelete(HttpRequest &request, HttpResponse &response);
	MyController();
	MyController(int masking);
	~MyController();

	std::string     doExecuteWrite(std::string &data,  std::string filename, const char *cgi_python);
	std::string     doExecuteRead(std::string &data, const char *cgi_python);
	std::string		doExecuteDelete(HttpRequest &request, std::string data, const char *cgi_python);
};
#endif