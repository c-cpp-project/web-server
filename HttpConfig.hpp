#ifndef HTTP_CONFIG_HPP
# define HTTP_CONFIG_HPP

#include<map>
#include<string>

class HttpConfig
{
private:
    static  std::map<std::string, std::string> statusCodeRepo;
public:
    HttpConfig();
    ~HttpConfig();
    static std::string  getHttpStatusMsg(std::string key);
    static void putHttpStatusCode(std::string key, std::string value);
    static  std::string pathResolver(std::string path);
};
#endif