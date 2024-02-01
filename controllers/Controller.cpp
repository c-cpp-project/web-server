#include"Controller.hpp"

void    Controller::service(HttpRequest &request, HttpResponse &response)
{
    std::cout << request.getHeader("CONTENT-TYPE") << ", " << request.getMethod() << "\n";
    if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
        doGet(request, response);
    else if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") == "multipart/form-data") // file upload
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


std::string Controller::getCurrentDate() {
	// 현재 시각 얻기
	std::time_t currentTime_t;
    std::time(&currentTime_t);

    // 시각을 현지 시간대로 변환
    std::tm* localTime = std::localtime(&currentTime_t);

	// 현재 날짜 및 시간 문자열 생성
	std::ostringstream oss;
	oss << std::put_time(localTime, "%a, %d %b %Y %H:%M:%S GMT");
	return oss.str();
}