#include "Handler.hpp"

std::map<int, std::string> Handler::buffers;  // 요청을 읽어오는 소켓, 버퍼
std::map<int, HttpRequest*> Handler::chunkeds;

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
  int remainingSize = serverConfig->getClientRequestSize("") - buffers[fd].size();
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

void Handler::removeAndDeleteChunkedRequest(int socket_fd) {
  HttpRequest* request = removeChunkedRequest(socket_fd);
  delete request;
}

HttpRequest* Handler::removeChunkedRequest(int socket_fd) {
  std::map<int, HttpRequest*>::iterator it = chunkeds.find(socket_fd);
  if (it == chunkeds.end()) return (NULL);
  HttpRequest* request = it->second;
  chunkeds.erase(it);
  return (request);
}

void Handler::errorHandling(const char* erorr_code) {
  HttpRequest empty;
  HttpResponse response(fd);
  response.setStatusCode(erorr_code);
  response.forward(empty);
  response.flush();
}

// chunked 상태 관리는 이벤트 처리를 다르게 하는 방식을 처리해야 할 것 같습니다.
// 따라서 enum 상태 값에 따라 event를 다르게 등록하는 방식으로 처리하고자
// 합니다.
int Handler::createHttpRequest() {
  HttpRequest* request = HttpRequestFactory::create(fd, serverConfig);
  if (request == NULL &&
      buffers[fd].size() == (size_t)serverConfig->getClientRequestSize(""))
    return FAILURE;  // TODO: 우선은 상수로 반환하고 나중에 try catch 하는
                     // 방식으로 바꾸기
  // throw SocketCloseException400();  // 제한된 크기를 초과하는 요청
  if (request == NULL)
    return FAILURE;  // 버퍼에 완전한 요청이 없거나 연결이 끊김
  return ChunkedRequestHandling(request);  // 상태에 따라 다르게 이벤트 등록하기
}

int Handler::ChunkedRequestHandling(HttpRequest* request) {
  if (request->getHeader("Transfer-Encoding") != "chunked")
    return (NO_CHUNKED_REQUEST);

  // chunkeds 맵에 포함된 chunked 요청인 경우 -> 아직 chunked 요청 전체가 끝나지
  // 않았음
  if (chunkeds.find(fd) != chunkeds.end()) return (IN_PROGRESS_CHUNKED_REQUEST);

  // 마지막 chunked 요청인 경우
  HttpRequestParser::parseRequestParams(request);
  return (LAST_CHUNKED_REQUEST);
}

void Handler::executeFrontController() {
  // kqueue_fd는 없어도 될 거 같습니다. 그렇지만 혹시 몰라서 그대로
  // 이용했습니다.
  int kqueue_fd = 0;
  FrontController front_controller(kqueue_fd, fd);
  front_controller.run(*request);
  delete request;
}