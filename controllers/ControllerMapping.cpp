#include"ControllerMapping.hpp"

std::map<std::pair<int, std::string>, Controller *> ControllerMapping::controllers;

void ControllerMapping::putController(int port, std::string uri, Controller *controller)
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

Controller *ControllerMapping::getController(int port, std::string uri)
{
	std::pair<int, std::string>	key;

	key.first = port;
	key.second = getLocationUri(uri);
	if (controllers[key] == 0)
	{
		key.first = 0;
		key.second = DEFAULT;
	}
	return (controllers[key]);
}

void		ControllerMapping::mapController(std::map<int, ServerConfiguration*> &serverConfigs)
{
	std::map<std::string, Location>::iterator		configIter;
	std::map<int, ServerConfiguration*>::iterator	portIter;			
	Server				server;
	std::stringstream 	ss;

	for (portIter = serverConfigs.begin(); portIter != serverConfigs.end(); portIter++)
	{
		int					port;
		ServerConfiguration	config;

		port = portIter->first;
		config = *portIter->second;
		server = config.getServer();
		for (configIter = server.getLocations().begin(); configIter != server.getLocations().end(); configIter++)
		{
			std::string				locationUri = configIter->first;
			std::set<std::string>	allowMethod = configIter->second.getAllowMethod();
			unsigned int			method;

			method = 0;
			if (allowMethod.find("GET") != allowMethod.end())
				method += METHOD::GET;
			if (allowMethod.find("POST") != allowMethod.end())
				method += METHOD::POST;
			if (allowMethod.find("DELETE") != allowMethod.end())
				method += METHOD::DELETE;
			putController(port, locationUri, new MyController(method));
		}
	}
	putController(0, DEFAULT, new DefaultController());
}

void		ControllerMapping::deleteController(void)
{
	std::map<std::pair<int, std::string>, Controller *>::iterator iter;

	for (iter = controllers.begin(); iter != controllers.end(); iter++)
	{
		delete iter->second;
	}
}