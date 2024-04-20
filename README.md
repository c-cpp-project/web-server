# nginx-like-webesrv

### 요구사항
1. one processor, one thread
2. I/O multiplexing by kqueue
3. using CGI script by Python to make Dynamic Web Page
4. HTTP 1.1 protocol: GET method, POST method, DELETE method, redirect, connectoin: keep-avlie, connectoin: close, transfer-encoding: chunked and etc

# I/O multiplexing: kqueue

# Recive: request 객체
1. 클라이언트 소켓으로부터 HTTP 요청이 들어온 경우, `HttpRequestHandler::handle` 함수에서 처리합니다.
2. **소켓으로부터 요청을 읽어들여 각 소켓 당 마련된 버퍼에 저장**하고, keep-alive에 의해 버퍼에 여러 개의 요청이 쌓여 있을 수 있음을 전제하여 처리합니다.
3. `HttpRequestFactory::create` 함수에서 **하나의 HTTP 요청에 대응하는 하나의 HttpRequest 객체를 생성**합니다.        
4. 요청 메시지를 파싱하고, **chunked 요청인지 또는 일반 요청인지 파악하여 서로 다른 처리 로직이 구성**됩니다. HttpRequest 객체를 만들어 바로 반환하는 일반 요청과 달리, chunked 요청은 본문이 여러 번에 걸쳐 들어올 수 있음을 고려합니다.
6. 요청 메시지를 파싱하는 도중, 요청이 유효하지 않음을 발견하면 예외를 던져 클라이언트에게 오류 응답을 합니다.
7. 만들어진 유효한 HttpRequest 객체는 Send 쪽으로 전달됩니다.

# Send: response 객체

# CGI script
