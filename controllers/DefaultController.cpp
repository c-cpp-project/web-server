#include"DefaultController.hpp"

DefaultController::DefaultController() : Controller(1)
{}

DefaultController::~DefaultController()
{}

void    DefaultController::service(HttpRequest &request, HttpResponse &response)
{
    std::string         redirectPath;
    std::string         staticPath;
    std::string         path;
    ServerConfiguration *serverConfig = response.getServerConfiguration();
    // 메서드 허용 여부 검사
    if (isAcceptableMethod(request.getMethod()) == false)
        throw "405";
    std::cout << "DefaultController::service" << "\n";
    redirectPath = serverConfig->getRedirectionPath(request.getPath()).second;
    if (redirectPath != "")
    {
        response.setStatusCode(serverConfig->getRedirectionPath(request.getPath()).first);
        request.setPath(redirectPath);
        response.redirect(request.getPath());
    }
    else
    {
        staticPath = request.getPath(); // 4. 없으면 무엇을 반환하는가? -> error를 던진다.
        std::cout << staticPath << " = staticPath\n";
        request.setPath(staticPath.substr(1));
        response.forward(request);
    }
}