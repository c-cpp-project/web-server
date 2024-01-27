

#include<string>
#include<cstring>

#ifndef ERROR_RESPONSE_HPP
# define ERROR_RESPONSE_HPP

class ErrorResponse
{
private:
    std::string statusCode;
    std::string statusMsg;
public:
    ErrorResponse(const std::string& code, const std::string& msg);
    const char* what() const throw()
    {
        return (statusCode.c_str());
    }
};
#endif
