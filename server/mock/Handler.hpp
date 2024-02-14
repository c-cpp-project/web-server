#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <unistd.h>

#include <map>
#include <string>

#include "../../HttpRequest/HttpRequestFactory.hpp"
#include "../Event.hpp"
#include "../ServerConfiguration.hpp"

#define SUCCESS 0
#define FAILURE -1

class Handler {
 private:
  int fd;
  ServerConfiguration* serverConfig;
  Event* eventHandler;
  static std::map<int, std::string> buffers;  // 요청을 읽어오는 소켓, 버퍼
  // static std::map<int, HttpRequest*>
  //     chunkeds;  // chunked 수신 중인 소켓, request 객체

 public:
  Event* events;
  Handler(int fd, ServerConfiguration* serverConfig, Event* currEvent);
  Handler();
  bool readMessage();
  int readRequest();
  void removeBuffer(int socket_fd);
};

#endif
