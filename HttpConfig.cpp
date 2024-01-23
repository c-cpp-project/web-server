#include"HttpConfig.hpp"
#include"controllers/ControllerMapping.hpp"

std::string HttpConfig::getHttpStatusMsg(std::string key)
{
	return (statusCodeRepo[key]);
}

void    HttpConfig::putHttpStatusCode(std::string key, std::string value)
{
	statusCodeRepo[key] = value;
}

HttpConfig::HttpConfig()
{
	int	i;
	std::string	key[31] = {
		"200", "201", "202", "206", "301", "302", "303", "307", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "500", "501", ""
	};
	std::string	value[31] = {
		"OK","Created","Accepted","Partial Content","Moved Permanently","Found","See Other","Temporary Redirect","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Internal Server Error","Not Implemented", ""
	};

	i = 0;
	while (key[i].length() != 0)
	{
		statusCodeRepo.insert({key[i], value[i]});
		i++;
	}
	ControllerMapping::putController("/fail", new FailController());
	ControllerMapping::putController("/login", new LoginController()); // login, file post
	// ControllerMapping.put("/hello", new Hello());
}

std::string HttpConfig::pathResolver(std::string path)
{
	if (path == "/")
		path = "index";
	return ("static/html" + path + ".html");
}

HttpConfig::~HttpConfig()
{

}