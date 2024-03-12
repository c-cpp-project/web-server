#include "ControllerMapping.hpp"

std::map<std::pair<std::pair<int, std::string>, std::string>, Controller *> ControllerMapping::controllers;

ControllerMapping::ControllerMapping(
		std::map<std::pair<std::string, int>, ServerConfiguration *>
				serverConfigs) {
	ControllerMapping::mapController(serverConfigs);  // Contorller 설정
	ControllerMapping::putController(0, "", DEFAULT,
																	 new DefaultController());  // defualt
}

ControllerMapping::~ControllerMapping() {
	ControllerMapping::deleteController();
}

void ControllerMapping::putController(int port, std::string servername, std::string uri, Controller *controller) {
	std::pair<int, std::string> server;

	server.first = port;
	server.second = servername;
	controllers.insert(std::make_pair(std::make_pair(server, uri), controller));
	// std::cout << "PUT CONTROLLER [" << port << ", " << servername << ", " << uri << " = " << controllers.size() << "]\n";
}

std::pair<std::pair<int, std::string>, std::string> ControllerMapping::getControllerKey(int port, std::string servername, std::string uri) {
	std::pair<std::pair<int, std::string>, std::string> key;
	std::string location;
	std::pair<int, std::string> server;

	server.first = port;
	server.second = servername;
	key.first = server;
	key.second = "";
	std::map<std::pair<std::pair<int, std::string>, std::string>, Controller *>::iterator iter;
	for (iter = controllers.begin(); iter != controllers.end(); iter++) {
		location = iter->first.second;
		if (iter->first.first != server || location.length() > uri.length()) continue ;
		if (location == uri.substr(0, location.length()) &&
				(location.length() == uri.length() || uri[location.length()] == '/')) {
			if (key.second.length() < location.length()) key.second = location;
		}
	}
	if (controllers[key] == 0) {
		key.first.first = 0;
		key.first.second = "";
		key.second = DEFAULT;
	}
	return (key);
}

Controller *ControllerMapping::getController(int port, std::string servername, std::string uri) {
	std::pair<std::pair<int, std::string>, std::string> key;
	std::cout << port << ", " << servername << ", " << uri << " = getController 1\n";
	key = getControllerKey(port, servername, uri);
	std::cout << key.first.first << ", " << key.first.second << ", " << key.second << " = getController 2\n";
	return (controllers[key]);
}

void ControllerMapping::mapController(
		std::map<std::pair<std::string, int>, ServerConfiguration *> &serverConfigs) {
	std::map<std::pair<std::string, int>, ServerConfiguration *>::iterator portIter;
	std::stringstream ss;

	for (portIter = serverConfigs.begin(); portIter != serverConfigs.end();
			 portIter++) {
		int         port = portIter->first.second;
		std::string servername = portIter->first.first;
		Server *server = portIter->second->getServer();
		std::map<std::string, Location *>::iterator configIter;
		for (configIter = server->getLocations().begin();
				 configIter != server->getLocations().end(); configIter++) {
			std::string locationUri = configIter->first;
			const std::set<std::string> allowMethod =
					configIter->second->getAllowMethod();
			unsigned int method;

			method = 0;
			if (allowMethod.find("GET") != allowMethod.end()) method += 1;
			if (allowMethod.find("POST") != allowMethod.end()) method += 2;
			if (allowMethod.find("DELETE") != allowMethod.end()) method += 8;

			putController(port, servername, locationUri, new MyController(method, locationUri));
		}
	}
}

void ControllerMapping::deleteController(void) {
	std::map<std::pair<std::pair<int, std::string>, std::string>, Controller *>::iterator iter;

	for (iter = controllers.begin(); iter != controllers.end(); iter++) {
		delete iter->second;
	}
}