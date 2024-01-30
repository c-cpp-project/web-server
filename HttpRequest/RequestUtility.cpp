#include "RequestUtility.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

// input string을 delimeter를 기준으로 스플릿해서 vector<string> 형태로 변환
std::vector<std::string> RequestUtility::splitString(const std::string& input, char delimeter)
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

// query string을 map<string, string> 형태로 변환
std::map<std::string, std::string> RequestUtility::parseQueryString(const std::string& query_string)
{
	if (containWhiteSpace(query_string)) // 화이트스페이스를 포함하는 쿼리스트링은 유효하지 않음
		throw std::invalid_argument("400 Bad Request");

	std::map<std::string, std::string> result;
	std::vector<std::string> pairs = splitString(query_string, '&');
	for(size_t i = 0; i < pairs.size(); i++)
	{
		std::vector<std::string> tockens = splitString(pairs[i], '=');
		if (tockens.size() != 2)
			throw std::invalid_argument("400 Bad Request");
		result[tockens[0]] = tockens[1];
	}
	return (result);
}

// string의 선&후행 화이트스페이스 제거
std::string RequestUtility::trim(std::string string, const char *to_remove)
{
	string.erase(0, string.find_first_not_of(to_remove));
	string.erase(string.find_last_not_of(to_remove) + 1);
	return (string);
}

std::string RequestUtility::toUpperString(const std::string& input)
{
	std::string result(input);
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return (result);
}

bool RequestUtility::containWhiteSpace(const std::string& input)
{
	for(size_t i = 0; i < input.size(); i++)
	{
		if (std::isspace(input[i]))
			return (true);
	}
	return (false);
}

bool RequestUtility::isExist(const std::map<std::string, std::string>& map, const std::string& key)
{
	if (map.find(key) == map.end())
		return (false);
	return (true);
}

int RequestUtility::toPositiveInt(const std::string& input)
{
	int result = 0;
	for(size_t i = 0; i < input.size(); i++)
	{
		if (input[i] < '0' || '9' < input[i])
			throw std::invalid_argument("400 Bad Request");
		result = result * 10 + input[i] -'0';
	}
	return (result);
}