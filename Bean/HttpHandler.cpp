#include"HttpHandler.hpp"

HttpHandler::HttpHandler()
{}

HttpHandler::HttpHandler(int fd, ServerConfiguration *serverConfig)
{
    this->fd = fd;
    this->serverConfig = serverConfig;
    this->data = "";
}

HttpHandler::HttpHandler(int fd, std::string data, ServerConfiguration *serverConfig)
{
    this->fd = fd;
    this->data = data;
    this->serverConfig = serverConfig;
}

HttpHandler::HttpHandler(int fd, HttpResponse res)
{
    this->fd = fd;
    this->serverConfig = NULL;
    this->response = res;
    this->data = "";
}

HttpHandler::HttpHandler(int fd, HttpRequest req, HttpResponse res)
{
    this->fd = fd;
    this->request = req;
    this->response = res;
    this->serverConfig = NULL;
}

HttpHandler::HttpHandler(int fd, HttpRequest req, HttpResponse res, ServerConfiguration *serverConfig)
{
    this->fd = fd;
    this->request = req;
    this->response = res;
    this->serverConfig = serverConfig;
    this->data = "";
}

HttpHandler::~HttpHandler()
{}

HttpHandler::HttpHandler(const HttpHandler& ref)
{
    fd = ref.fd;
    request = ref.request;
    response = ref.response;
    serverConfig = ref.serverConfig;
    data = ref.data;
}

HttpHandler&	HttpHandler::operator=(const HttpHandler& ref)
{
    if (this == &ref)
        return (*this);
    fd = ref.fd;
    request = ref.request;
    response = ref.response;
    serverConfig = ref.serverConfig;
    data = ref.data;
    return (*this);
}

int				HttpHandler::getFd(void)
{
    return (fd);
}

HttpRequest&    HttpHandler::getHttpRequest(void)
{
    return ((request));
}

HttpResponse&   HttpHandler::getHttpResponse(void)
{
    return ((response));
}

ServerConfiguration*	HttpHandler::getServerConfiguration(void)
{
    return (serverConfig);
}

std::string		HttpHandler::getData(void)
{
    return (this->data);
}