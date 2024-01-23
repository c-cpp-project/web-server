#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

#include<exception>
#include<iostream>

class FrontController
{
private:
    int socketfd;
    int keepalive_timeout; // 65s
public:
    FrontController(int socketfd);
    FrontController();
    ~FrontController();

    void    run();
};
#endif