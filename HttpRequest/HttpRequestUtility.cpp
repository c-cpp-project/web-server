#include "HttpRequestUtility.hpp"
#include <string>
#include <sstream>

// input string을 delimeter를 기준으로 스플릿해서 vector<string> 형태로 변환
std::vector<std::string> HttpRequestUtility::splitString(const std::string& input, char delimeter)
{
	std::vector<std::string> result;
	std::istringstream iss(input);

	std::string buffer;
	while (std::getline(iss, buffer, delimeter))
	{
		if (buffer != "")
			result.push_back(buffer);
	}

	return (result);
}

std::vector<std::string> HttpRequestUtility::splitStringOnce(const std::string& input, char delimeter)
{
	std::vector<std::string> result;
	std::istringstream iss(input);

	std::string buffer;
	std::getline(iss, buffer, delimeter);
	
	result.push_back(buffer);
}

// query string을 map<string, string> 형태로 변환
std::map<std::string, std::string> HttpRequestUtility::parseQueryString(const std::string& query_string)
{
	std::map<std::string, std::string> result;

	std::vector<std::string> pairs = splitString(query_string, '&');
	for(size_t i = 0; i < pairs.size(); i++)
	{
		std::vector<std::string> tockens = splitString(pairs[i], '=');
		if (tockens.size() != 2)
		{
			throw std::invalid_argument("invalid query string \"" + query_string + "\"");
		}
		result[tockens[0]] = tockens[1];
	}
	return (result);
}

// string의 선&후행 화이트스페이스 제거
std::string HttpRequestUtility::trim(std::string string, const char *to_remove)
{
	string.erase(0, string.find_first_not_of(to_remove));
	string.erase(string.find_last_not_of(to_remove) + 1);
	return (string);
}