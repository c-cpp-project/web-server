
#include<exception>
#include<iostream>
#include"../HttpConfig.hpp"
#include"../controllers/Controller.hpp"
#include"../HttpRequest/HttpRequest.hpp"
#include<string>
#include<sys/fcntl.h>

#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

class FrontController
{
private:
    int socketfd;
    // int keepalive_timeout; // 65s
public:
    FrontController(int socketfd);
    FrontController();
    ~FrontController();

    void    run();
};
#endif