

#include<exception>
#include<iostream>

#ifndef FRONT_CONTROLLER_HPP
# define FRONT_CONTROLLER_HPP

class FrontController
{
private:
    int socketfd;
    // int skeepalive_timeout; // 65s
public:
    FrontController(int socketfd);
    FrontController();
    ~FrontController();

    void    run();
};
#endif