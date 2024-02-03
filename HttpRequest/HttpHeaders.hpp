#ifndef HTTP_HEADERS_HPP
# define HTTP_HEADERS_HPP

# include "RequestUtility.hpp"

class HttpHeaders
{
	private:
		std::map<std::string, std::string> headers;
		static bool weakDuplicationBan(const std::string& field);
		static bool strongDuplicationBan(const std::string& field);

	public:
		void addHeader(const std::string& line);
		std::string getHeader(const std::string& field);
		void setHeader(const std::string& field, const std::string& value);

		// 테스트용
		std::map<std::string, std::string>::iterator getBegin();
		std::map<std::string, std::string>::iterator getEnd();
};

#endif