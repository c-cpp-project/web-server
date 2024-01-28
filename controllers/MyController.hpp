#ifndef MyController_HPP
# define MyController_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <vector>
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
	void	doDelete(HttpRequest &request, HttpResponse &response);
    std::string	doExecute(HttpRequest &request, std::string data, const char *cgi_python);
    std::string getCurrentDate();
    MyController();
    ~MyController();

    std::string    doExecuteLarge(std::string &data, const char *cgi_python);
    std::string    doExecuteSmall(std::string &data, const char *cgi_python);
};
#endif