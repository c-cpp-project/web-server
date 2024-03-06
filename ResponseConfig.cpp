#include"ResponseConfig.hpp"

std::map<std::string, std::string>  ResponseConfig::statusCodeRepo;
std::map<std::string, std::string>  ResponseConfig::cgiAddressRepo;
std::set<std::string>				ResponseConfig::supportedTypesRepo;

ResponseConfig::ResponseConfig()
{
	setHttpStatusCode(); // StatusCode 초기화 설정
	setHttpContentType();
}

ResponseConfig::~ResponseConfig()
{}

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
	std::string	key[35] = {
		"405", "200", "201", "202", "206", "301", "302", "303", "307", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "400", "401", "404", "406", "408", "411", "413", "414", "416", "431", "500", "501", "417", ""
	};
	std::string	value[35] = {
		"Method Not Allowed", "OK","Created","Accepted","Partial Content","Moved Permanently","Found","See Other","Temporary Redirect","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Bad Request","Unauthorized","Not Found","Not Acceptable","Request Timeout","Length Required","Payload Too Large","URI Too Long","Requested Range Not Satisfiable","Request Header Fields Too Large","Internal Server Error","Not Implemented", "Expectation Failed", ""
	};

	i = 0;
	while (key[i].length() != 0)
	{
		statusCodeRepo.insert(std::make_pair(key[i], value[i]));
		i++;
	}
}

std::string ResponseConfig::getCurrentDate() 
{
    std::time_t currentTime_t;
    std::time(&currentTime_t);

    std::tm* localTime = std::localtime(&currentTime_t);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", localTime);
    return buffer;
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
	supportedTypesRepo.insert("image/x-icon");

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
	int i;

	std::cout << "content type: " << filename << "\n";
	contentType = "text/plain";
	for (i = filename.length() - 1; i >= 0; i--)
	{
		if (filename[i] == '.')
			break ;
	}
	extension = filename.substr(i + 1);
	std::cout << "extension: " << extension << "\n";
	for (iter = supportedTypesRepo.begin(); iter != supportedTypesRepo.end(); iter++)
	{
		if ((*iter).find(extension) != std::string::npos)
		{
			contentType = *iter;
			break ;
		}
	}
	std::cout << "ResponseConfig::getContentType: " << contentType << "\n";
	return (contentType);
}

//================================================================================================//
//									필수적인 설정 부분												   //
//================================================================================================//