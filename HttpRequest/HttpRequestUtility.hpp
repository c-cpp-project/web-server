#ifndef HTTP_REQUEST_UTILITY_HPP
# define HTTP_REQUEST_UTILITY_HPP

# include <iostream>
# include <vector>
# include <map>

class HttpRequestUtility
{
	public:
		static std::vector<std::string> splitString(const std::string& input, char delimeter);
		static std::vector<std::string> splitStringOnce(const std::string& input, char delimeter);
		static std::map<std::string, std::string> parseQueryString(const std::string& query_string);
		static std::string trim(std::string string, const char *to_remove = " \t\n\v\f\r");
};

#endif