#include "HttpRequestUtility.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

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

std::string HttpRequestUtility::toUpperString(const std::string& input)
{
	std::string result(input);
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return (result);
}

bool HttpRequestUtility::containWhiteSpace(const std::string& input)
{
	for(size_t i = 0; i < input.size(); i++)
	{
		if (std::isspace(input[i]))
			return (true);
	}
	return (false);
}

bool HttpRequestUtility::isImpossibleDuplication(const std::string& header_field)
{
	// TODO : 이후 중복이 불가능한 헤더에 대해서만 true, 그 외에는 false를 반환하도록 수정하기
	if (header_field == "") return (false); // 임시
	return (true);
}