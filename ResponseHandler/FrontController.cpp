#include"FrontController.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include<string>

void    FrontController::run()
{
    HttpRequest     request(this->socketfd);
    HttpResponse    response(this->socketfd);
    // HttpResponse    response(this->socketfd, 64);
    std::string     uri;
    Controller      *controller;

    uri = request.getPath();
    try
    {
        controller = ControllerMapping::getController(uri);
        if (controller == 0 || (request.getMethod() == "GET" && request.getQueryString() == ""))
        {
            if (HttpConfig::IsRedriectUri(uri) == true) // redirect
                response.redirect(request, response);
            else
                response.forward(request, response); // get
        }
        else // cgi: GET && POST, FILE POST, DELETE
        {
            std::cout << uri <<  ", " << request.getParameter("username") << ", " << request.getParameter("password") << "\n"; 
            controller->service(request, response);
        }
    }
    catch(char const* e)
    {
        std::cerr << e << '\n';
        response.setStatusCode(e);
        response.forward(request, response);
    }
}

FrontController::FrontController(int socketfd)
{
    this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}