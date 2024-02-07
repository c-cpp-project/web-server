#ifndef CONTROLLER_MAPPING_HPP
# define CONTROLLER_MAPPING_HPP

#include<map>
#include"Controller.hpp"
#include"MyController.hpp"
#include"../server/ServerConfiguration.hpp"

class Controller;
class ControllerMapping
{
private:
	static std::map<std::pair<std::string, std::string>, Controller *> controllers;
public:
	static	void		mapController();
	static	void		putController(std::string port, std::string location, Controller *controller); // config file에서 설정
	static	Controller *getController(std::string port, std::string uri); // uri에 대응하는 controller 뽑기
	static	std::string	ControllerMapping::getLocationUri(std::string uri); // uri에 대응하는 Location 얻기
};
#endif