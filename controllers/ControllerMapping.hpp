#ifndef CONTROLLER_MAPPING_HPP
#define CONTROLLER_MAPPING_HPP
#include <map>

#include "../server/ServerConfiguration.hpp"
#include "Controller.hpp"
#include "DefaultController.hpp"
#include "MyController.hpp"

#define DEFAULT "default"
class Controller;
class ControllerMapping {
 private:
  static std::map<std::pair<std::pair<int, std::string>, std::string>, Controller *> controllers;
  ControllerMapping();

 public:
  ControllerMapping(std::map<std::pair<std::string, int>, ServerConfiguration *> serverConfigs);
  ~ControllerMapping();
  static void deleteController();
  static std::pair<std::pair<int, std::string>, std::string> getControllerKey(int port, std::string servername, std::string uri);
  static void mapController(
      std::map<std::pair<std::string, int>, ServerConfiguration *> &serverConfigs);
  static void putController(int port, std::string servername, std::string uri, Controller *controller);
  static Controller *getController(
      int port, std::string servername, std::string uri);  // uri에 대응하는 controller 뽑기
};
#endif
