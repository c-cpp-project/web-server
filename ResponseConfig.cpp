#include"ResponseConfig.hpp"

std::string ResponseConfig::serverName = "WebServe";
std::map<std::string, std::string>  ResponseConfig::statusCodeRepo;
std::map<std::string, std::string>  ResponseConfig::pathRepo;
std::map<std::string, std::string>  ResponseConfig::redirectRepo;
std::map<std::string, std::string>  ResponseConfig::cgiAddressRepo;

std::string ResponseConfig::getHttpStatusMsg(std::string key)
{
	return (statusCodeRepo[key]);
}

std::string	ResponseConfig::getRedirectPath(std::string srcUri)
{
	std::string	destUri;

	destUri = redirectRepo[srcUri];
	return (pathResolver(destUri));
}

void    ResponseConfig::putHttpStatusCode(std::string key, std::string value)
{
	statusCodeRepo[key] = value;
}

ResponseConfig::ResponseConfig()
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
	redirectRepo.insert(std::make_pair("/redirect", "/hello"));

}

// pathResolver는 redirect 요청이나 get 요청을 할 때 사용한다.
// uri에 대응하는 nginx file을 적용하여 값을 반환 
std::string ResponseConfig::pathResolver(std::string uri)
{
	std::cout << "[" << uri << "]\n";
	return (uri);
	// if (access(uri.substr(1).c_str(), F_OK) == 0)
	// 	return (uri.substr(1));
	// if (uri == "/")
	// 	uri = "/welcome";
	// // std::string uri;
	// // path = pathRepo.get("uri")
	// // return (path);
	// return ("static/html" + uri + ".html");
}

bool    ResponseConfig::IsRedriectUri(std::string srcUri)
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

std::string  ResponseConfig::getServerName()
{
	return (ResponseConfig::serverName);
}

ResponseConfig::~ResponseConfig()
{
	// static 동적 할당 모두 삭제
	Controller *controller = ControllerMapping::getController("/controller");
	delete controller;
}

std::string ResponseConfig::getCurrentDate() {
	// 현재 시각 얻기
	std::time_t currentTime_t;
    std::time(&currentTime_t);

    // 시각을 현지 시간대로 변환
    std::tm* localTime = std::localtime(&currentTime_t);

	// 현재 날짜 및 시간 문자열 생성
	std::ostringstream oss;
	oss << std::put_time(localTime, "%a, %d %b %Y %H:%M:%S GMT");
	return oss.str();
}