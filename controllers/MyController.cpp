#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::~MyController()
{}

// parameter
void    MyController::doGet(HttpRequest &request, HttpResponse &response)
{
    
}

// file post
void	MyController::doPost(HttpRequest &request, HttpResponse &response)
{

}

// delete
void	MyController::doDelete(HttpRequest &request, HttpResponse &response)
{
    std::string currentDate = getCurrentDate();
    std::string path;
    int         fd;
    const char* fileName = HttpConfig::pathResolver(request.getPath()).c_str();

    if (access(fileName, W_OK) == -1) // Unauthorized -> Access 권한이 없없다다.
        throw ErrorResponse("401", HttpConfig::getHttpStatusMsg("401"));
    // ================== CGI 구현 사항 ==========================//
    
    if (std::remove(fileName) != 0) //  Internal Server Error -> 삭제 할 수 없다.
        throw ErrorResponse("500", HttpConfig::getHttpStatusMsg("500"));

    // ================== CGI 구현 사항 ==========================//
    response.setStatusCode("204"); // if the action has been enacted and no further information is to be supplied.
    response.ResponseStatusLine();
    response.putHeader("Server", HttpConfig::getServerName());
    response.putHeader("Date", getCurrentDate());
    response.sendBody("");
}

std::string MyController::getCurrentDate() {
    // 현재 시각 얻기
    auto currentTime = std::chrono::system_clock::now();

    // 시각을 현지 시간대로 변환
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm* localTime = std::localtime(&currentTime_t);

    // 현재 날짜 및 시간 문자열 생성
    std::ostringstream oss;
    oss << std::put_time(localTime, "%a, %d %b %Y %H:%M:%S GMT");
    return oss.str();
}