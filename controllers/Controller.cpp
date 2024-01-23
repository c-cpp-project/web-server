#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    // ControllerMapping::getController(uri);
    // delete, redirect, post -> 성공하면 hello.html, 실패하면 fail.html
    // uri를 보고 적합한 controller를 실행한다.
    std::string status_code;

    status_code = response.getStatusCode();
    if (request.getMethod() == "GET" || status_code[0] == '4' || status_code[0] == '5') // error
        doGet(request, response);
    else if (request.getMethod() == "POST")
        doPost(request, response);
    else if (request.getMethod() == "DELETE")
        doDelete(request, response);
    else
        throw ErrorResponse("404", HttpConfig::getHttpStatusMsg("404"));
}

Controller::Controller()
{}

Controller::~Controller()
{}