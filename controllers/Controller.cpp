#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    if (request.getHeader("Content-Type") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST" && request.getHeader("Content-Type") == "multipart/form-data") // file upload
        doPost(request, response);
    else if (request.getMethod() == "DELETE") // delete
        doDelete(request, response);
    else // Bad Request
        throw ErrorResponse("400", HttpConfig::getHttpStatusMsg("400"));
}

Controller::Controller()
{}

Controller::~Controller()
{}