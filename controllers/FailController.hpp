#ifndef FAIL_CONTROLLER_HPP
# define FAIL_CONTROLLER_HPP

#include"Controller.hpp"

class FailController : public Controller
{
private:
public:
    void    doGet(HttpRequest &request, HttpResponse &response);
    FailController();
    ~FailController();
};
#endif