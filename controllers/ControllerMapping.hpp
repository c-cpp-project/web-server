#ifndef CONTROLLER_MAPPING_HPP
# define CONTROLLER_MAPPING_HPP

#include<map>
#include"Controller.hpp"
#include"MyController.hpp"
#include"DefaultController.hpp"
#include"../server/ServerConfiguration.hpp"

#define	DEFAULT "default"
class Controller;
class ControllerMapping
{
private:
	static std::map<std::pair<int, std::string>, Controller *> controllers;
public:
	static	void		deleteController();
	static	void		mapController(std::map<int, ServerConfiguration*> &serverConfigs);
	static	void		putController(int port, std::string location, Controller *controller); // config file에서 설정
	static	Controller *getController(int port, std::string uri); // uri에 대응하는 controller 뽑기
	static	std::string	getLocationUri(std::string uri); // uri에 대응하는 Location 얻기
};
#endif