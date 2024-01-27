
#ifndef MyController_HPP
# define MyController_HPP

#include"Controller.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include<vector>

class Controller;
class MyController : public Controller
{
private:
    /* data */
public:
    void    doGet(HttpRequest &request, HttpResponse &response);
	void	doPost(HttpRequest &request, HttpResponse &response);
	void	doDelete(HttpRequest &request, HttpResponse &response);
    std::string	doExecute(HttpRequest &request, std::string data, char *cgi_python);
    std::string getCurrentDate();
    MyController();
    ~MyController();

    std::string    doExecuteLarge(HttpRequest &request, std::string &data, char *cgi_python);
    std::string    doExecuteSmall(HttpRequest &request, std::string &data, char *cgi_python);
};
#endif