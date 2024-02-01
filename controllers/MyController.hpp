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
    std::string	doExecute(HttpRequest &request, std::string data, const char *cgi_python);
    MyController();
    ~MyController();

    std::string    doExecuteLarge(std::string &data, const char *cgi_python);
    std::string    doExecuteSmall(std::string &data, const char *cgi_python);
};
#endif