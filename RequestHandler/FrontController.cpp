#include"FrontController.hpp"
#include"HttpRequest.hpp"
#include"HttpResponse.hpp"
#include"../controllers/ControllerMapping.hpp"
#include<string>

void    FrontController::run()
{
    HttpRequest     request;
    HttpResponse    response(this->socketfd);
    std::string uri;
    Controller *controller;

    uri = request.getPath();
    controller = ControllerMapping::getController(uri);
    try
    {
        if (controller == 0) // view: get -> html 보기
            response.forward(request, response);
        else // api: delete, post (o), fail (o), file post, cgi
            controller->service(request, response);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        response.setStatusCode(e.what());
        response.ResponseStatusLine();
        controller = ControllerMapping::getController("/fail");
        controller->service(request, response);
    }
    delete controller;
}

FrontController::FrontController(int socketfd)
{
    this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}

// int main(void)
// {

//     // ... kqueue와 설정 처리
//     // event가 발생한 socket에 관하여 http 통신
//     FrontController  FrontController(socketfd);
//     FrontController.run(); // request와 response에 대한 처리 완료
// }