#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    // 메서드 허용 여부 검사
    if (isAcceptableMethod(request.getMethod()) == false)
        throw "400";
    // request.setPath(); 경로 수정하기
    if (request.getMethod() == "GET" && request.getQueryString() == "")
    {
        if (ResponseConfig::IsRedriectUri(request.getPath()) == true) // redirect
            response.redirect(request, response);
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
{
    this->masking = 1;
}

Controller::Controller(int masking)
{
    this->masking = masking;
}

Controller::~Controller()
{}

void	    Controller::response200(std::string body, HttpResponse &response)
{
	std::stringstream ss;
	std::string	bodyLength;

	response.putHeader("Server", ResponseConfig::getServerName());
	response.putHeader("Date", ResponseConfig::getCurrentDate());
	response.putHeader("Content-Type", "text/html;charset=utf-8");
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
	response.sendBody(body);
}

bool    Controller::isAcceptableMethod(std::string method)
{
    if (method == "GET" && (masking & 1))
        return (true);
    else if (method == "POST" && (masking & 2))
        return (true);
    else if (method == "DELETE" && (masking & 4))
        return (true);
    else
        return (false);
}