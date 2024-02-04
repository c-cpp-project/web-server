#ifndef MY_CONTROLLER_HPP
# define MY_CONTROLLER_HPP

#include"Controller.hpp"

class HttpResponse;
class HttpRequest;
class MyController : public Controller
{
private:
    /* data */
public:
    void    doGet(HttpRequest &request, HttpResponse &response);
	void	doPost(HttpRequest &request, HttpResponse &response);
    MyController();
    ~MyController();

    std::string     doExecuteWrite(std::string &data,  std::string filename, const char *cgi_python);
    std::string     doExecuteRead(std::string &data, const char *cgi_python);
};
#endif