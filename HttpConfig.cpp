#include"HttpConfig.hpp"
#include"controllers/ControllerMapping.hpp"

std::string HttpConfig::getHttpStatusMsg(std::string key)
{
	return (statusCodeRepo[key]);
}

std::string	HttpConfig::getRedirectPath(std::string srcUri)
{
	std::string	destUri;

	destUri = redirectRepo[srcUri];
	return (pathResolver(destUri));
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
	// pathRepo.put()
	// redirectRepo.put("/src_redirect", "/dest_redirect");
	// ControllerMapping.put("/hello", new Hello()); -> CGI에서 처리해야 할 요청을 모두 저장
	// 예를 들어, ControllerMapping.put()"form 관련 요청 uri", new FromController());
	// 하지만 귀찮기 때문에 하나에 다 박자.
	ControllerMapping::putController("/index", new MyController());
}

// pathResolver는 redirect 요청이나 get 요청을 할 때 사용한다.
// uri에 대응하는 nginx file을 적용하여 값을 반환 
std::string HttpConfig::pathResolver(std::string uri)
{
	if (uri == "/")
		uri = "index";
	// std::string uri;
	// path = pathRepo.get("uri")
	// return (path);
	return ("static/html" + uri + ".html");
}

bool    HttpConfig::IsRedriectUri(std::string srcUri)
{
	std::map<std::string, std::string>::iterator it = std::find(redirectRepo.begin(), redirectRepo.end(), srcUri);

    if (it != redirectRepo.end())
        return (true);
	return (false);
}

std::string  HttpConfig::getServerName()
{
	return (HttpConfig::serverName);
}

HttpConfig::~HttpConfig()
{

}