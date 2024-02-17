#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::MyController(int masking) : Controller(masking)
{}

MyController::~MyController()
{}

void    MyController::service(HttpRequest &request, HttpResponse &response)
{
    std::string originPath;
    std::string path;
    ServerConfiguration *serverConfig = response.getServerConfiguration();

    if (isAcceptableMethod(request.getMethod()) == false)
        throw "405";
    originPath = request.getPath();
    path = serverConfig->getResourcePath(originPath);
    if (path != "")
        request.setPath(path);
    if (request.getMethod() == "GET" && request.getQueryString() == "")
    {
        if (serverConfig->getRedirectionPath(originPath).second != "") // redirect
            response.redirect(request.getPath());
        else
            response.forward(request); // get
    }
    else if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST") // file upload
        doPost(request, response);
    else
        doDelete(request, response);
}