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
    std::string			extension = serverConfig->getCgiTestExt();
	bool				allowedMethod;

    allowedMethod = false;
    if (request.getHeader("connection") == "close")
		response.putHeader("connection", "close");
	else
		response.putHeader("connection", "");
	if (request.getPath().length() >= extension.length() && \
	request.getPath().substr(request.getPath().length() - extension.length()) == extension)
	{
		if (request.getMethod() == "POST")
			allowedMethod = true;
	}
	if (isAcceptableMethod(request.getMethod()) == false && allowedMethod == false)
		throw "405";
    std::cout << "DefaultController::service" << "\n";
    // 메서드 허용 여부 검사
    std::cout << "============ 1 serverConfig->getPort() ========\n";
    std::cout << request.getPath() << "\n";
    std::cout << "[" << serverConfig->getRedirectionPath(request.getPath()).second << "]\n";
    std::cout << "============ 2 serverConfig->getPort() ========\n";
    if (isAcceptableMethod(request.getMethod()) == false)
        throw "405";
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
        if (staticPath == "/favicon.ico")
            staticPath = "static" + staticPath;
        else
            staticPath = staticPath.substr(staticPath.find("/") + 1);
        std::cout << staticPath << " = staticPath\n";
        request.setPath(staticPath);
        response.putHeader("Content-Type", ResponseConfig::getContentType(request.getPath()));
        response.forward(request);
    }
}