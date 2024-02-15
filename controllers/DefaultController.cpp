#include"DefaultController.hpp"


DefaultController::DefaultController() : Controller(METHOD::GET)
{}

DefaultController::~DefaultController()
{}

void    DefaultController::service(HttpRequest &request, HttpResponse &response, ServerConfiguration *serverConfig)
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
    if (ResponseConfig::IsRedriectUri(originPath) == true) // redirect
        response.redirect(originPath, response);
    else
        response.forward(request, response); // get
}