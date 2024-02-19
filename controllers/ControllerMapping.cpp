#include"ControllerMapping.hpp"

std::map<std::pair<int, std::string>, Controller *> ControllerMapping::controllers;

ControllerMapping::ControllerMapping(std::map<int, ServerConfiguration*> serverConfigs)
{
	ControllerMapping::mapController(serverConfigs); // Contorller 설정
	ControllerMapping::putController(0, DEFAULT, new DefaultController()); // defualt
}

ControllerMapping::~ControllerMapping()
{
	ControllerMapping::deleteController();
}

void ControllerMapping::putController(int port, std::string uri, Controller *controller)
{
	controllers.insert(std::make_pair(std::make_pair(port, uri), controller));
}

std::string	ControllerMapping::getLocationUri(std::string uri)
{
	size_t		idx;
	std::string	location;

	idx = uri.substr(1).find('/');
	location = uri.substr(idx);
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
	std::stringstream 	ss;

	std::cout << "size: " << serverConfigs.size() << "\n";
	for (portIter = serverConfigs.begin(); portIter != serverConfigs.end(); portIter++)
	{
		int				port = portIter->first;
		const Server	&server = portIter->second->getServer();
		
		std::cout << "port: " << port << "\n";
		for (configIter = server.getLocations().begin(); configIter != server.getLocations().end(); configIter++)
		{
			std::string					locationUri = configIter->first;
			const std::set<std::string>	allowMethod = configIter->second.getAllowMethod();
			unsigned int				method;

			method = 0;
			if (allowMethod.find("GET") != allowMethod.end())
				method += 1;
			if (allowMethod.find("POST") != allowMethod.end())
				method += 2;
			if (allowMethod.find("DELETE") != allowMethod.end())
				method += 8;
			putController(port, locationUri, new MyController(method));
		}
	}
}

void		ControllerMapping::deleteController(void)
{
	std::map<std::pair<int, std::string>, Controller *>::iterator iter;

	for (iter = controllers.begin(); iter != controllers.end(); iter++)
	{
		delete iter->second;
	}
}