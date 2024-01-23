#include"LoginController.hpp"

// index.html에서 form형식 2개
// 1. 파일 업로드
// 2. 로그인: 처음이거나 (user, passwd) 맞으면 ok, (user, passwd)에서 passwd가 다르면 fail
void	LoginController::doPost(HttpRequest &request, HttpResponse &response)
{
    if (request.getHeader("Content-Type") == "application/x-www-form-urlencoded")
    {
        std::string bodyLength = "2";
	    std::string	body = "ok";
        
        response.putHeader("Content-Type", "text/html;charset=utf-8");
        if (Repository::isLogin(request.getParameter("username"), request.getParameter("password")) == false)
        {
            body = "fail";
            bodyLength = "4";
        }
        response.putHeader("Content-Length", bodyLength);
        response.sendBody(body);
    }
    // else // file uploade

}

LoginController::LoginController() : Controller()
{}

LoginController::~LoginController()
{}