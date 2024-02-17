#include"ResponseConfig.hpp"

std::map<std::string, std::string>  ResponseConfig::statusCodeRepo;
std::map<std::string, std::string>  ResponseConfig::cgiAddressRepo;
std::set<std::string>				ResponseConfig::supportedTypesRepo;

ResponseConfig::ResponseConfig()
{}

ResponseConfig::ResponseConfig(std::map<int, ServerConfiguration*> &serverConfigs, Event *event)
{
	setHttpStatusCode(); // StatusCode 초기화 설정
	ControllerMapping::mapController(serverConfigs, event); // Contorller 설정
	ControllerMapping::putController(0, DEFAULT, new DefaultController()); // defualt
}

ResponseConfig::~ResponseConfig()
{
	ControllerMapping::deleteController();
}

std::string ResponseConfig::getHttpStatusMsg(std::string key)
{
	return (statusCodeRepo[key]);
}

void    ResponseConfig::putHttpStatusCode(std::string key, std::string value)
{
	statusCodeRepo[key] = value;
}

void	ResponseConfig::setHttpStatusCode(void)
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

void	ResponseConfig::setHttpContentType(void)
{
	// text 타입
	supportedTypesRepo.insert("text/html");
	supportedTypesRepo.insert("text/plain");
	supportedTypesRepo.insert("text/css");
	supportedTypesRepo.insert("text/javascript");  // 또는 application/javascript

  	// 애플리케이션 타입
	supportedTypesRepo.insert("application/json");
	supportedTypesRepo.insert("application/xml");
	supportedTypesRepo.insert("application/x-www-form-urlencoded");
	supportedTypesRepo.insert("application/pdf");
	supportedTypesRepo.insert("application/octet-stream");

	// 이미지 타입
	supportedTypesRepo.insert("image/jpeg");
	supportedTypesRepo.insert("image/png");
	supportedTypesRepo.insert("image/gif");
	supportedTypesRepo.insert("image/webp");

	// 오디오/비디오 타입
	supportedTypesRepo.insert("audio/mpeg");
	supportedTypesRepo.insert("audio/ogg");
	supportedTypesRepo.insert("video/mp4");
	supportedTypesRepo.insert("video/webm");
}

std::string	ResponseConfig::getContentType(std::string filename)
{
	std::set<std::string>::iterator	iter;
	std::string	extension;
	std::string	contentType;

	contentType = "text/plain";
	extension = filename.substr(filename.find(".") + 1);
	for (iter = supportedTypesRepo.begin(); iter != supportedTypesRepo.end(); iter++)
	{
		if ((*iter).find(extension) != std::string::npos)
		{
			contentType = *iter;
			break ;
		}
	}
	return (contentType);
}

//================================================================================================//
//									필수적인 설정 부분												   //
//================================================================================================//