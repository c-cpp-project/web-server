#include"HttpConfig.hpp"

std::string HttpConfig::serverName = "WebServe";
std::map<std::string, std::string>  HttpConfig::statusCodeRepo;
std::map<std::string, std::string>  HttpConfig::pathRepo;
std::map<std::string, std::string>  HttpConfig::redirectRepo;
std::map<std::string, std::string>  HttpConfig::cgiAddressRepo;

std::string HttpConfig::testBody = 
"-----------------------------1234567890123456789012345678\r\n\
Content-Disposition: form-data; name=\"text_field\"\r\n\
\r\n\
This is a simple text field.\r\n\
-----------------------------1234567890123456789012345678\r\n\
Content-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\r\n\
Content-Type: text/plain\r\n\
\r\n\
Contents of the file go here.\r\n\
-----------------------------1234567890123456789012345678--\r\n";

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
	std::string	key[32] = {
		"200", "201", "202", "206", "301", "302", "303", "307", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "500", "501", "417", ""
	};
	std::string	value[32] = {
		"OK","Created","Accepted","Partial Content","Moved Permanently","Found","See Other","Temporary Redirect","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Internal Server Error","Not Implemented", "Expectation Failed", ""
	};

	i = 0;
	while (key[i].length() != 0)
	{
		statusCodeRepo.insert(std::make_pair(key[i], value[i]));
		i++;
	}
	// pathRepo.put()
	redirectRepo.insert(std::make_pair("/redirect", "/welcome"));
	// ControllerMapping.insert("/hello", new Hello()); -> CGI에서 처리해야 할 요청을 모두 저장
	// 예를 들어, ControllerMapping.insert()"form 관련 요청 uri", new FromController());
	ControllerMapping::putController("/controller", new MyController());
}

// pathResolver는 redirect 요청이나 get 요청을 할 때 사용한다.
// uri에 대응하는 nginx file을 적용하여 값을 반환 
std::string HttpConfig::pathResolver(std::string uri)
{
	std::cout << "[" << uri << "]\n";
	if (access(uri.substr(1).c_str(), F_OK) == 0)
		return (uri.substr(1));
	if (uri == "/")
		uri = "/welcome";
	// std::string uri;
	// path = pathRepo.get("uri")
	// return (path);
	return ("static/html" + uri + ".html");
}

bool    HttpConfig::IsRedriectUri(std::string srcUri)
{
	std::cout << srcUri << ", size: " << redirectRepo.size() << "\n";
	std::map<std::string, std::string>::iterator it;
	try
	{
		it = redirectRepo.find(srcUri);
		if (it != redirectRepo.end())
        	return (true);
	}
	catch(const std::exception& e)
	{
		std::cerr << "IsRedriectUri: " << e.what() << '\n';
	}
	return (false);
}

std::string  HttpConfig::getServerName()
{
	return (HttpConfig::serverName);
}

HttpConfig::~HttpConfig()
{
	// static 동적 할당 모두 삭제
	Controller *controller = ControllerMapping::getController("/index");
	delete controller;
}

