#include"ControllerMapping.hpp"

std::map<std::string, Controller *> ControllerMapping::controllers;

void ControllerMapping::putController(std::string uri, Controller *controller)
{
	controllers.insert(std::make_pair(uri, controller));
}

Controller *ControllerMapping::getController(std::string uri)
{
	unsigned long	i;
	std::string		token;

	i = 1;
	while (i < uri.length() && uri[i] != '/')
		i++;
	token = uri.substr(0, i);
	return (controllers[token]);
}