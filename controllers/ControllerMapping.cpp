#include"ControllerMapping.hpp"

std::map<std::pair<std::string, std::string>, Controller *> ControllerMapping::controllers;

void ControllerMapping::putController(std::string port, std::string uri, Controller *controller)
{
	controllers.insert(std::make_pair(std::make_pair(port, uri), controller));
}

std::string	ControllerMapping::getLocationUri(std::string uri)
{
	size_t		idx;
	std::string	location;

	idx = uri.substr(1).find('/');
	location = uri.substr(1, idx);
	return (location);
}

Controller *ControllerMapping::getController(std::string port, std::string uri)
{
	std::pair<std::string, std::string>	key;

	key.first = port;
	key.second = getLocationUri(uri);
	return (controllers[key]);
}

void		ControllerMapping::mapController()
{
	ServerConfiguration	serverConfig;
	Server				server;
	std::map<std::string, Location>::iterator	iter;

	server = serverConfig.getServer();
	for (iter = server.getLocations().begin(); iter != server.getLocations().end(); iter++)
	{
		std::string		locationUri = iter->first;
		unsigned int	allowMethod = iter->second.getAllowMethod();

	}
}