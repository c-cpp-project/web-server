#include"FrontController.hpp"

void    FrontController::run()
{
    std::vector<HttpRequest> 	*request;
    MultiRequest                multiRequest(this->tmp);

    request = multiRequest.makeRequest();
    for (int i = 0; i < request->size(); i++)
    {
        HttpResponse    response(this->socketfd);
        Controller      *controller;

        controller = request->at(i).getMethod() == "DELETE" ? (new DeleteController()) : (ControllerMapping::getController(request->at(i).getPath()));
        if (controller == 0 || (request->at(i).getMethod() == "GET"))
        {
            if (HttpConfig::IsRedriectUri(request->at(i).getPath()) == true) // redirect
                response.redirect(request->at(i), response);
            else
                response.forward(request->at(i), response); // get
        }
        else // cgi: GET && POST, FILE POST, DELETE
        {
            controller->service(request->at(i), response);
            delete controller;
        }
        response.flush();
    }
    delete request;
}

FrontController::FrontController(int kqueuefd, int socketfd, HttpRequest &request)
{
    this->kqueuefd = kqueuefd;
    this->socketfd = socketfd;
    this->tmp = request;
}

FrontController::FrontController(int socketfd)
{
    this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}