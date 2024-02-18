
#include <map>
#include <set>

#include "IServerConfiguration.hpp"
#include "setting/Server.hpp"
class ServerConfiguration {
 private:
  std::set<std::string> defaultAllowedMethods;
  std::set<std::string> supportedTypes;
  std::map<int, int> serverSocketPortMap;
  Server* server;
  const std::string getPathByRootAndValue(std::string root, std::string value) const;

 public:
  ServerConfiguration();
  ServerConfiguration(Server& server);
  ~ServerConfiguration();
  //서버 설정에서 listen 포트번호를 의미한다. 설정파일에서 설정안하면 80
  int getPort() const;
  //keep-alive 옵션의 TCP 연결 지속시간
  int getKeepAliveTimeout();
  //max client body size 설정파일에서 설정안하면 10000
  long getClientBodySize() const;
  //max client header size 설정파일에서 설정안하면 10000
  long getClientHeaderSize() const;
  //max client body size + max client header size
  long getClientRequestSize() const;
  //서버 이름 설정 파일에서 설정안하면 default 값""
  const std::string getServerName() const;
  //해당 uri를 넣으면 접근해야할 정적 리소스 경로 반환
  const std::string getResourcePath(std::string uri) const;
  //해당 에러 상태코드를 넣으면 반환할 errorpage 경로 (ex. fail.html) 반환
  //해당 에러 상태코드에 대한 반환할 error page가 없으면 "" 반환
  const std::string getErrorpageResourcePath(int statusCode) const;
  //해당 uri를 넣으면 std::string 상태코드, std::string 리다이렉션 경로 반환
  //만약 리다이렉션 경로 없으면 pair "400", "" 반환 
  const std::pair<std::string, std::string> getRedirectionPath(
      const std::string& uri) const;
  //"application/json"과 같이 Content-Type을 std::string으로 넣으면 해당 contentType이 지원되는지 반환
  bool hasContentType(const std::string& contentType);
  //Server 객체에 직접 접근 가능
  const Server& getServer() const;
  //정적자원을 찾는 최상위 폴더인 root 반환
  const std::string getRoot() const;
  //파일 업로드 경로 반환
  const std::string getUploadPath() const;
  //해당 uri 넣으면 Location 존재할 때는 허용 메소드를 std::string 집합으로 반환
  //그게 아니면 GET 메소드 집합만 반환
  const std::set<std::string> getAllowedMethod(const std::string& uri) const;
  const std::string getCgiPath() const;
};

