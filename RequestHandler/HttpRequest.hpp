#include<string>

class HttpRequest
{
private:
    /* data */
public:
    std::string getMethod();
    std::string getPath();
    HttpRequest(/* args */);
    ~HttpRequest();
    RequestMethod getMethod() const
    {
        return (request_line->getMethod());
    }

    std::string getPath() const
    {
        return (request_line->getPath());
    }
    std::string getQueryString() const
    {
        return (request_line->getQueryString());
    }
    std::string getHeader(std::string header) const
    {
        return (request_headers->getHeader(header));
    }
    std::string getParameter(std::string param) const
    {
        return (request_params->getParameter(param));
    }
};
