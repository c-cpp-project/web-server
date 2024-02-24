
#ifndef HTTP_CONFIG_HPP
# define HTTP_CONFIG_HPP
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <iomanip>
#include <sstream>
#include <iostream>

class ServerConfiguration;
class Event;
class ResponseConfig
{
private:
	void    setHttpStatusCode(void);
	void	setHttpContentType(void);
	static  std::map<std::string, std::string>  statusCodeRepo;
	static	std::set<std::string>				supportedTypesRepo;


	static  std::map<std::string, std::string>  cgiAddressRepo;
public:
	ResponseConfig();
	~ResponseConfig();
	ResponseConfig(std::map<int, ServerConfiguration*> &serverConfigs, Event *event);
	static void putHttpStatusCode(std::string key, std::string value);

	static  std::string getCurrentDate();
	static  std::string	getContentType(std::string filename);
	static  std::string	getHttpStatusMsg(std::string key);
};

#endif