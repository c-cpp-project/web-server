#include "SessionStorage.hpp"
#include "../HttpRequest/RequestUtility.hpp"

std::map<std::string, SessionData*> SessionStorage::storage;

std::string SessionStorage::addSessionData(SessionData *session_data)
{
	std::string session_id;
	while (true)
	{
		session_id = generateSessionId();
		if (storage.find(session_id) == storage.end()) {
			storage[session_id] = session_data;
			break;
		}
	}
	return (session_id);
}

SessionData *SessionStorage::getSessionData(const std::string& session_id)
{
	if (storage.find(session_id) != storage.end())
		return (storage[session_id]);
	return (NULL);
}

std::string SessionStorage::generateSessionId()
{
	const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::srand(std::time(0));

	std::string session_id;
	for(int i = 0; i < 16; i++)
		session_id += charset[std::rand() % charset.length()];
	
	return (session_id);
}