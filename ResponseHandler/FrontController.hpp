
#include<exception>
#include<iostream>
#include"../HttpConfig.hpp"
#include"../controllers/Controller.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include"HttpResponse.hpp"
#include<string>
#include<sys/fcntl.h>

#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

class HttpRequest;
class HttpResponse;
class FrontController
{
private:
    int socketfd;
    int kqueuefd;
    HttpRequest request;
    HttpResponse response;
public:
    FrontController(int socketfd);
    FrontController(HttpRequest &request, HttpResponse &response);
    FrontController(int kqueuefd, HttpRequest &request, HttpResponse &response);
    FrontController();
    ~FrontController();

    void    run();
};
#endif