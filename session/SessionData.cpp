#include "SessionData.hpp"
#include "../HttpRequest/RequestUtility.hpp"

std::string SessionData::getValue(const std::string& key)
{
	if (RequestUtility::isExist(data, key))
		return (data[key]);
	return ("");
}

void SessionData::add(const std::string& key, const std::string& value)
{
	data[key] = value;
}
