#include"FrontController.hpp"

void    FrontController::run()
{
    HttpRequest     request(this->socketfd);
    HttpResponse    response(this->socketfd);
    // HttpResponse    response(this->socketfd, 64);
    std::string     uri;
    Controller      *controller = 0;

    uri = request.getPath();
    try
    {
        controller = request.getMethod() == "DELETE" ? \
        (new DeleteController()) : (ControllerMapping::getController(uri));
        if (controller == 0 || (request.getMethod() == "GET"))
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
            delete controller;
        }
    }
    catch(char const* e)
    {
        std::cerr << e << '\n';
        response.setStatusCode(e);
        response.forward(request, response);
        if (controller)
            delete controller;
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