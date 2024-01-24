#include"ControllerMapping.hpp"

void ControllerMapping::putController(std::string uri, Controller *controller)
{
    controllers.insert({uri, controller});
}

Controller *ControllerMapping::getController(std::string uri)
{
   return (controllers[uri]);
}