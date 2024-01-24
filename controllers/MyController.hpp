
#ifndef MyController_HPP
# define MyController_HPP

#include"Controller.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

class Controller;
class MyController : public Controller
{
private:
    /* data */
public:
    void    doGet(HttpRequest &request, HttpResponse &response);
	void	doPost(HttpRequest &request, HttpResponse &response);
	void	doDelete(HttpRequest &request, HttpResponse &response);
    std::string getCurrentDate();
    MyController();
    ~MyController();
};
#endif