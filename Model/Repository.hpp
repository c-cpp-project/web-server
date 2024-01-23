
#include<string>
#include<map>

class Repository
{
private:
    static std::map<std::string, std::string > repo;
    Repository();
public:
    ~Repository();
    // 최초의 user 등록
    static void    addRepository(std::string user, std::string passwd);
    // 처음이거나 (user, passwd) 맞으면 ok, (user, passwd)에서 passwd가 다르면 fail
    static bool isLogin(std::string user, std::string passwd);
    // user와 passwd가 동일한 user를 삭제
    static std::string delRepository(std::string user, std::string passwd);
};
