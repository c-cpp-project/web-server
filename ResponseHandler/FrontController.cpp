#include"FrontController.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"../controllers/ControllerMapping.hpp"
#include<string>

void    FrontController::run()
{
    HttpRequest     request(this->socketfd);
    HttpResponse    response(this->socketfd);
    std::string     uri;
    Controller      *controller;
    std::string     fileName = HttpConfig::pathResolver(uri);

    uri = request.getPath();
    try
    {
        if (access(fileName.c_str(), F_OK) == -1 && request.getMethod() != "POST") // Not Found
            throw ErrorResponse("404", HttpConfig::getHttpStatusMsg("404"));
        controller = ControllerMapping::getController(uri);
        if (controller == 0 || (request.getMethod() == "GET" && request.getQueryString() == ""))
        {
            if (HttpConfig::IsRedriectUri(uri) == true) // redirect
                response.redirect(request, response);
            else
                response.forward(request, response); // get
        }
        else // cgi: GET && POST, FILE POST, DELETE
            controller->service(request, response);
    }
    catch(const ErrorResponse& e)
    {
        std::cerr << e.what() << '\n';
        response.setStatusCode(e.what());
        response.forward(request, response);
    }
    delete controller;
}

FrontController::FrontController(int socketfd)
{
    this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}