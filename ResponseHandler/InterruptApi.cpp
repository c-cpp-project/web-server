#include"InterruptApi.hpp"

void    InterruptApi::callResponse()
{
    HttpResponse    response(sockfd);
    char            body[64 * K];
    size_t          ret;
    

    ret = read(pipefd, body, this->max_size);
    body[ret] = 0;
    if ("500" == std::string(body))
        throw "500";
    else
    {
        std::stringstream ss;
	    std::string	bodyLength;

        response.ResponseStatusLine();
        response.putHeader("Server", HttpConfig::getServerName());
        response.putHeader("Date", HttpConfig::getCurrentDate());
        response.putHeader("Content-Type", "text/html;charset=utf-8");
        ss << ret;
        bodyLength = ss.str();
        response.putHeader("Content-Length", bodyLength);
        response.sendBody(body);
    }
}

InterruptApi::InterruptApi(int pipefd, int sockfd)
{
    this->pipefd = pipefd;
    this->sockfd = sockfd;
    this->max_size = 64 * K;
}

InterruptApi::InterruptApi()
{}

InterruptApi::~InterruptApi()
{}