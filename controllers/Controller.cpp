#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    std::cout << request.getHeader("CONTENT-TYPE") << ", " << request.getMethod() << "\n";
    if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST") // file upload
        doPost(request, response);
    else if (request.getMethod() == "DELETE") // delete
        doDelete(request, response);
    else // Bad Request
        throw "400";
}

void    Controller::doGet(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

void	Controller::doPost(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
    request.getMethod();
    response.getMaxBodySize();
}

Controller::Controller()
{}

Controller::~Controller()
{}

void	    Controller::response200(std::string body, HttpResponse &response)
{
	std::stringstream ss;
	std::string	bodyLength;

	response.putHeader("Server", HttpConfig::getServerName());
	response.putHeader("Date", HttpConfig::getCurrentDate());
	response.putHeader("Content-Type", "text/html;charset=utf-8");
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
	response.sendBody(body);
}