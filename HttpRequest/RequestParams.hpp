#ifndef REQUEST_PARAMS_HPP
# define REQUEST_PARAMS_HPP

# include "RequestUtility.hpp"

class RequestParams
{
	private:
		std::map<std::string, std::string> params;

	public:
		void addQuearyString(const std::string& query_string);
		// void addJsonData();
		std::string getParameter(const std::string& param);

		// 테스트용
		std::map<std::string, std::string>::iterator getBegin();
		std::map<std::string, std::string>::iterator getEnd();
};

#endif