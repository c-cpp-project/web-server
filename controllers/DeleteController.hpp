#ifndef DELETE_CONTROLLER_HPP
# define DELETE_CONTROLLER_HPP

#include"Controller.hpp"

class HttpResponse;
class HttpRequest;
class DeleteController : public Controller
{
private:
public:
    std::string	doExecute(HttpRequest &request, std::string data, const char *cgi_python);
    void        doDelete(HttpRequest &request, HttpResponse &response);
    DeleteController();
    ~DeleteController();
};
#endif