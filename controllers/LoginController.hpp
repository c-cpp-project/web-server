// login -> 성공하면, 

#include"Controller.hpp"
#include"../Model/Repository.hpp"
class LoginController : public Controller
{
private:
    
public:
    void	doPost(HttpRequest &request, HttpResponse &response);

    LoginController();
    ~LoginController();
};
