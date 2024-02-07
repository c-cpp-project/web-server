#ifndef REQUEST_PARAMS_HPP
# define REQUEST_PARAMS_HPP

# include "RequestUtility.hpp"

class RequestParams
{
	private:
		std::map<std::string, std::string> params;

	public:
		int addQuearyString(const std::string& query_string);
		int addCookie(const std::string& cookie_string);

		std::string getParameter(const std::string& param);
		std::string getCookieValue(const std::string& key);

		// 테스트용
		std::map<std::string, std::string>::iterator getBegin();
		std::map<std::string, std::string>::iterator getEnd();
};

#endif