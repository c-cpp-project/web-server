#include "SessionData.hpp"
#include "../HttpRequest/RequestUtility.hpp"

std::string SessionData::getValue(const std::string& key)
{
	std::string upper_key = RequestUtility::toUpperString(key);
	if (RequestUtility::isExist(data, upper_key))
		return (data[upper_key]);
	return ("");
}

void SessionData::add(const std::string& key, const std::string& value)
{
	std::string upper_key = RequestUtility::toUpperString(key);
	data[upper_key] = value;
}
