#ifndef REQUEST_UTILITY_HPP
# define REQUEST_UTILITY_HPP

# include <iostream>
# include <vector>
# include <map>

# define SUCCESS 0
# define FAILURE -1

class RequestUtility
{
	public:
		static std::vector<std::string> splitString(const std::string& input, char delimeter);
		static std::map<std::string, std::string> parseQueryString(const std::string& query_string, bool& success);
		static std::string trim(std::string string, const char *to_remove = " \t\n\v\f\r");
		static std::string toUpperString(const std::string& input);
		static bool containWhiteSpace(const std::string& input);
		static bool isExist(const std::map<std::string, std::string>& map, const std::string& key);
		static long strToPositiveLong(const std::string& input);
		static std::string positiveLongToStr(long input);
		static long hexToDecimal(const std::string &hex_string);
};

#endif