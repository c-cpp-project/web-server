# 핵심 변경 사항

## Controller

1. MyController: nginx 설정 파일에서 uri의 경로가 location에 등록된 경우, Request가 허용 메서드인 경우 내부적으로 적절한 uri로 변환 후에 Controller 실행
2. DefaultController: nginx 설정 파일에서 uri의 경로가 등록되지 않은 경우, GET 메서드로 uri 그대로 실행한다.

## ResponseConfig.hpp
1. Controller를 설정하기 위한 Response 설정 파일로 serverConfigs을 생성자의 매개변수로 받아 std::map에 등록한다.

## default.conf에 맞게 설정값 변경하기

# 추가 변경 사항

## transfer-encoding:chunked
"HTTP 1.1의 문자열" 중복 여부 검사에서 "sendBody의 중복 검사"로 변경 이를 통해 response message는 항상 하나임을 예외없이 구현 가능하다.

## Range-request
get에서 일부 데이터만 보내준다.

## Cookie-Sesstion

## Controller.run
포인터 주소를 넘기자.

# 파일을 NON_BLOCK

### http socket read요청
request 만들기 -> 정적파일 읽을 필요가 있다.

### 정적파일 이벤트 등록
EV_SET ident fd , udata this

### fd 이벤트 발생
정적파일을 읽는다는 이벤트 - http 요청 fd 구분 (Read && !socketFd)
1. 정적 파일 진행
2. udata 처음 요청했던 클랙스 객체 포인터 -> http response -> set body
3. http socket fd write 이벤트 등록

### socket 이벤트 발생
udata를 가지고 적절히 읽어서 보내라.
fd, (void *)udata Handler;

# EVENT
### 1: http socket read
1. 이벤트 등록: yuikim씨에게 알아서 잘 하심
2. 이벤트 발생: Reuqest 작업을 기존대로 하면 된다.
3. 순차적인 이벤트의 등록에서 udata: Request만 보내면 된다. (class: Reqeust와 Response) 요청 담당하는 클래스 - http fd 하나당 handler 객체를 생성 -> 포인터 이벤트 등록 

HTTP 메시지를 읽고 파싱
- GET 쿼리스트링 없는 경우 : 정적파일 읽는 이벤트 등록
- GET 쿼리스트링 있는 경우, DELETE : CGI READ 이벤트 등록 
- POST 파일 업로드 CGI WRITE 이벤트 등록 : CGI WRITE, READ 이벤트 등록 
- Handler에서 httpRequest, httpResponse 필드로 두는 걸로 바꾸기(제안)

### 2: CGI WRITE
1. 이벤트 등록: CGI처리를 해야 할 때, 이벤트 등록
2. 이벤트 발생: socket fd인지 확인했는데, 아니야. 그러면 CGI에 적어

- 이벤트 발생하면, 파일 업로드만 하면 됨!

### 3: CGI READ, 정적파일
1. 이벤트 등록: fd를 이벤트 등록
2. 이벤트 발생: socket fd인지 확인했는데, 아니야, response body에 적어. 그리고 http socket write 이벤트 등록
3. 순차적인 이벤트의 등록에서 udata: Request 전체와 body 보내야 한다. (class: Reqeust와 Response)

# 궁금한 사항
### 이벤트 등록에 관한 함수

void Event::saveEvent(int fd, int16_t filter, uint16_t flags, uint32_t fflags,
                      intptr_t data, void* udata);
// Event.cpp 참고:

### handler를 어떻게 만들지?

1. fd: cgi read

httphandler -> cgi, file send -> request & repsonse; 

request, response -> cgi, file send => response -> response;;;;;;;;;;;;;;;

yuikim -> http socket request, response


std::map<int, *httpHandler> umap

class httpHandler 
{
    Event* event; //Event.cpp
    std::vector<Request> requests; rq rq rq rq
    Response response;
    
    메소드들 모두 넣음
    1. cgi 관련
    2. 정적 파일 관련 메소드
}

udata = httpHandler
class httpHandler
{
    request; // fd
    response; // fd
}

셍상자 파라미터
* int clientFd
* ServerConfiguration* serverConfig
* Event* currEvent

HttpHandler 필드
* request <- fd 담기게끔 생성
* response <- fd 담기게끔 생성
* serverConfig
* currEvent

1. 등록: 
currEvent.saveEvent(옵션들...);
2. 등록할 때 udata: request, response
httpHandler
메소드 호출
this->request
this->response 

test() {
    this->reqeust
    this->response
}

cgi read 이벤트 발생
HttpHandler cgiRead() 호출

void cgiRead() {
    this->request 
    this->response
}
 
### file open (NON_BLOCK)과 fcntl NONBLOCK: file open도 NONBLOCK으로 열 수 있는가?

...
---

http recv 이벤트 발생하면, handler -> "httpRequest -> cgi, 정적파일 body I/O 작업" -> response

request -> I/O 이벤트 등록; -> CGI READ 실행
request -> I/O 이벤트 등록; -> flush 실행
request -> I/O 이벤트 등록; -> ... 
            flush 이벤트 등록;

# Request
close(socketfd);는 yuikim씨 쪽에서 처리한다.
return (-1);

# Response
handler에 상태 추가 
1. socketfd를 닫는 경우
2. socketfd를 닫지 않는 경우
