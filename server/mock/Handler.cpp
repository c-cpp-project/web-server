#include "Handler.hpp"

std::map<int, std::string> Handler::buffers;  // 요청을 읽어오는 소켓, 버퍼
// std::map<int, HttpRequest*> Handler::chunkeds;

// 매개변수가 있는 생성자의 구현
Handler::Handler(int fd, ServerConfiguration* serverConfig, Event* eventHandler)
    : fd(fd), serverConfig(serverConfig), eventHandler(eventHandler) {
  // 여기에 필요한 초기화 코드를 작성할 수 있습니다.
}

// 기본 생성자의 구현
Handler::Handler()
    : fd(-1) {  // fd를 -1로 초기화하여 "유효하지 않음"을 나타냅니다.
  // 여기에 기본 초기화 코드를 작성할 수 있습니다.
}

bool Handler::readMessage() {
  const size_t BUFFER_SIZE = 65536;
  char buffer[BUFFER_SIZE];
  ssize_t readSize = read(fd, buffer, BUFFER_SIZE);
  write(1, buffer, readSize);
  return true;
};

int Handler::readRequest() {
  int remainingSize = serverConfig->getClientRequestSize() - buffers[fd].size();
  char* temp_buffer =
      new char[remainingSize];  // 여유 크기만큼 버퍼 메모리 할당

  int read_byte = read(fd, temp_buffer, remainingSize);
  if (read_byte <= 0) {
    delete[] temp_buffer;
    // TODO: read_byte가 -1일 때만 socket exception을 던지는 이유가 있는지?
    return (FAILURE);  // 클라이언트와 연결 끊김
    // TODO : 클라이언트 측에서 연결을 끊은 경우, 소켓 닫기 & buffer, chunked
    // 삭제는 kqueue 쪽에서 해주려나?
  }

  buffers[fd] += std::string(temp_buffer, read_byte);
  delete[] temp_buffer;

  return (SUCCESS);
}

void Handler::removeBuffer(int socket_fd) {
  std::map<int, std::string>::iterator it = buffers.find(socket_fd);
  if (it != buffers.end()) buffers.erase(it);
}

// void Handler::removeAndDeleteChunkedRequest(int socket_fd) {
//   HttpRequest* request = removeChunkedRequest(socket_fd);
//   delete request;
// }