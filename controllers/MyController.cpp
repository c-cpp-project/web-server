#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::MyController(int masking) : Controller(masking)
{}

MyController::~MyController()
{}

void    MyController::service(HttpRequest &request, HttpResponse &response, ServerConfiguration *serverConfig)
{
    std::string originPath;
    std::string path;
    // 메서드 허용 여부 검사
    if (isAcceptableMethod(request.getMethod()) == false)
        throw "405";
    originPath = request.getPath();
    path = serverConfig->getResourcePath(originPath);
    if (path != "")
        request.setPath(path);
    if (request.getMethod() == "GET" && request.getQueryString() == "")
    {
        if (ResponseConfig::IsRedriectUri(originPath) == true) // redirect
            response.redirect(originPath, response);
        else
            response.forward(request, response); // get
    }
    else if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST") // file upload
        doPost(request, response);
    else
        doDelete(request, response);
}