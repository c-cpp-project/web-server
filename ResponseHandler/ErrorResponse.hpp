#include<exception>
#include<string>

class ErrorResponse : public std::exception 
{
private:
    std::string code;
    std::string message;

public:
    ErrorResponse(const std::string& code, const std::string& msg);
    const char *what() const throw();
};