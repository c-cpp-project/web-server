#ifndef CONTROLLER_MAPPING_HPP
# define CONTROLLER_MAPPING_HPP

#include<map>
#include"Controller.hpp"
#include"MyController.hpp"

class Controller;
class ControllerMapping
{
private:
    static std::map<std::string, Controller *> controllers;
public:
    static void putController(std::string uri, Controller *controller); // config file에서 설정
    static Controller *getController(std::string uri); // uri에 대응하는 controller 뽑기
};
#endif