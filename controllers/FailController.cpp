#include"FailController.hpp"

FailController::FailController() : Controller()
{}

FailController::~FailController()
{}

void    FailController::doGet(HttpRequest &request, HttpResponse &response)
{
    int			fd;
	std::string bodyLength;
	std::stringstream ss;
	std::string	body;

    fd = open(HttpConfig::pathResolver("/fail").c_str(), O_RDONLY);
    body = response.readFile(fd);
    ss << body.length();
	bodyLength = ss.str();
    response.putHeader("Content-Type", "text/html;charset=utf-8");
    response.putHeader("Content-Length", bodyLength);
    response.sendBody(body);
}