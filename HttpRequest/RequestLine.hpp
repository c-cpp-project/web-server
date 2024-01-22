#ifndef REQUEST_LINE_HPP
# define REQUEST_LINE_HPP

# include "HttpRequestUtility.hpp"

class RequestLine
{
	private:
		std::string method;
		std::string path;
		std::string query_string;

		void parseMethod(std::string method_string);
		void parseURI(std::string uri_string);

	public:
		RequestLine(const std::string& input);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQueryString() const;
};

#endif