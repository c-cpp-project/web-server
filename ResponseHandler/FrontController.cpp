#include"FrontController.hpp"

void    FrontController::run()
{
    std::string     uri;
    Controller      *controller = 0;

    try
    {
        uri = request.getPath();
        controller = request.getMethod() == "DELETE" ? (new DeleteController()) : (ControllerMapping::getController(uri));
        if (controller == 0 || (request.getMethod() == "GET"))
        {
            if (HttpConfig::IsRedriectUri(uri) == true) // redirect
                response.redirect(request, response);
            else
                response.forward(request, response); // get
        }
        else // cgi: GET && POST, FILE POST, DELETE
        {
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
    response.flush();
}

FrontController::FrontController(int kqueuefd, HttpRequest &request, HttpResponse &response)
{
    this->kqueuefd = kqueuefd;
    this->request = request;
    this->response = response;
}


FrontController::FrontController(HttpRequest &request, HttpResponse &response)
{
    this->request = request;
    this->response = response;
}

FrontController::FrontController(int socketfd)
{
    this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}