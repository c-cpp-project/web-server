#include"DefaultController.hpp"

DefaultController::DefaultController() : Controller(METHOD::GET)
{}

DefaultController::~DefaultController()
{}

void    DefaultController::service(HttpRequest &request, HttpResponse &response)
{
    std::string         originPath;
    std::string         path;
    ServerConfiguration *serverConfig = response.getServerConfiguration();
    // 메서드 허용 여부 검사
    if (isAcceptableMethod(request.getMethod()) == false)
        throw "405";
    originPath = request.getPath();
    path = serverConfig->getResourcePath(originPath);
    if (path != "")
        request.setPath(path);
    if (serverConfig->getRedirectionPath(originPath).second != "") // redirect
        response.redirect(request.getPath());
    else
        response.forward(request); // get
}