#include "StringUtils.hpp"

std::string StringUtils::toString(int num) {
    std::stringstream sstream;
	sstream << num;
	return sstream.str();
}
