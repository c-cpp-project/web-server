#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <unistd.h>

#include <map>
#include <string>

#include "../../HttpRequest/HttpRequestFactory.hpp"
#include "../../ResponseHandler/FrontController.hpp"
#include "../Event.hpp"
#include "../ServerConfiguration.hpp"

#define SUCCESS 0
#define FAILURE -1

#define NO_CHUNKED_REQUEST 1
#define IN_PROGRESS_CHUNKED_REQUEST 2
#define LAST_CHUNKED_REQUEST 3

class Handler {
 private:
  int fd;
  ServerConfiguration* serverConfig;
  Event* eventHandler;
  static std::map<int, std::string> buffers;  // 요청을 읽어오는 소켓, 버퍼
  static std::map<int, HttpRequest*>
      chunkeds;  // chunked 수신 중인 소켓, request 객체
  HttpRequest* request;  // 응답 보내는 부분이랑 요청 만드는 부분을 함수로 따로
                         // 호출하게 되는데 그 때 필요할거 같아요.

 public:
  Event* events;
  Handler(int fd, ServerConfiguration* serverConfig, Event* currEvent);
  Handler();
  bool readMessage();
  int readRequest();
  void removeBuffer(int socket_fd);
  static HttpRequest* removeChunkedRequest(int socket_fd);
  static void removeAndDeleteChunkedRequest(int socket_fd);
  void errorHandling(const char* erorr_code);
  int ChunkedRequestHandling(HttpRequest* request);

  // handle 부분에서 따로 함수로 뺀 것
  void executeFrontController();
  int createHttpRequest();
};

#endif
