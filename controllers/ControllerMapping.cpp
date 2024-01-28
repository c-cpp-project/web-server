#include"ControllerMapping.hpp"

std::map<std::string, Controller *> ControllerMapping::controllers;

void ControllerMapping::putController(std::string uri, Controller *controller)
{
    controllers.insert(std::make_pair(uri, controller));
}

Controller *ControllerMapping::getController(std::string uri)
{
   return (controllers[uri]);
}