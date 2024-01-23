#include<ErrorResponse.hpp>

ErrorResponse::ErrorResponse(const std::string &code, const std::string &message)
{
    this->code = code;
    this->message = message;
}

const char* ErrorResponse::what()
const throw()
{
    std::string msg;

    msg = this->code;
    return msg.c_str();
}
