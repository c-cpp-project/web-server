#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "INginxConfiguration.hpp"
#include "SocketUtils.hpp"

class WebServer {
public:
    static WebServer& getInstance(INginxConfiguration& nginxConfig);
    void run();

private:
    const INginxConfiguration& nginxConfig;
    WebServer(const INginxConfiguration& nginxConfig);
    WebServer(const WebServer&);
    WebServer& operator=(const WebServer&);
};

#endif 
