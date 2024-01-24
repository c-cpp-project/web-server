#ifndef HTTP_CONFIG_HPP
# define HTTP_CONFIG_HPP

#include<map>
#include<string>

static  std::string serverName = "WebServe";
class HttpConfig
{
private:
    static  std::map<std::string, std::string>  statusCodeRepo;
    static  std::map<std::string, std::string>  pathRepo;
    static  std::map<std::string, std::string>  redirectRepo;
    static  std::string serverName;
public:
    HttpConfig();
    ~HttpConfig();
    static std::string  getHttpStatusMsg(std::string key);
    static void putHttpStatusCode(std::string key, std::string value);
    static  std::string pathResolver(std::string uri);
    static  bool    IsRedriectUri(std::string srcUri);
    static  std::string  getRedirectPath(std::string srcUri);
    static  std::string  getServerName();
};
#endif