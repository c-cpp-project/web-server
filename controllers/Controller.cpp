#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    std::cout << request.getHeader("Content-Type") << ", " << request.getMethod() << "\n";
    if (request.getHeader("Content-Type") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST" && request.getHeader("Content-Type") == "multipart/form-data") // file upload
        doPost(request, response);
    else if (request.getMethod() == "DELETE") // delete
        doDelete(request, response);
    else // Bad Request
        throw "400";
}

void    Controller::doGet(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

void	Controller::doPost(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

Controller::Controller()
{}

Controller::~Controller()
{}