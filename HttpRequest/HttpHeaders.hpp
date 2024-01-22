#ifndef HTTP_HEADERS_HPP
# define HTTP_HEADERS_HPP

# include "HttpRequestUtility.hpp"

class HttpHeaders
{
	private:
		std::map<std::string, std::string> headers;

	public:
		void addHeader(const std::string& line);
		std::string getHeader(const std::string& header);

		// 테스트용
		std::map<std::string, std::string>::iterator getBegin();
		std::map<std::string, std::string>::iterator getEnd();
};

#endif