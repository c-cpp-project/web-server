#ifndef SESSION_DATA_HPP
# define SESSION_DATA_HPP

# include <map>

class SessionData
{
	private:
		std::map<std::string, std::string> data;
	
	public:
		std::string getValue(const std::string& key);
		void add(const std::string& key, const std::string& value);
};

#endif