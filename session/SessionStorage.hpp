#ifndef SESSION_STORAGE_HPP
# define SESSION_STORAGE_HPP

# include "SessionData.hpp"
# include <ctime>

class SessionStorage
{
	private:
		static std::map<std::string, SessionData*> storage; // 세션 ID, 세션 데이터
		static std::string generateSessionId();
	
	public:
		static std::string addSessionData(SessionData *session_data);
		static SessionData *getSessionData(const std::string& session_id);
};

#endif