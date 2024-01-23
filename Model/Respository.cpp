#include"Repository.hpp"

Repository::Repository()
{}

Repository::~Repository()
{
    Repository::repo.clear();
}

void    Repository::addRepository(std::string user, std::string passwd)
{
    std::map<std::string, std::string>::iterator it;
    
    it = Repository::repo.find(user);
    if (it == Repository::repo.end())
        Repository::repo.insert({user, passwd});
}

bool Repository::isLogin(std::string user, std::string passwd)
{
    std::map<std::string, std::string>::iterator it;

    it = Repository::repo.find(user);
    if (it != Repository::repo.end() && repo[user] != passwd)
        return (false);
    addRepository(user, passwd);
    return (true);
}

std::string Repository::delRepository(std::string user, std::string passwd)
{
    std::map<std::string, std::string>::iterator it;
    
    it = Repository::repo.find(user);
    if (it != Repository::repo.end() && Repository::repo[user] == passwd)
    {
        Repository::repo.erase(user);
        return (user);
    }
    return ("");
}
