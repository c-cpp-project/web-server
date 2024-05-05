# nginx-like-webesrv

### 요구사항
1. one processor, one thread
2. I/O multiplexing by kqueue
3. using CGI script by Python to make Dynamic Web Page
4. HTTP 1.1 protocol: GET method, POST method, DELETE method, redirect, connection: keep-alive, connectoin: close, transfer-encoding: chunked and etc

# I/O Multiplexing
1. I/O Multiplexing을 통해 여러 소켓과 파일 fd(file descriptor)를 동시에 관리할 수 있습니다.
2. kqueue()는 이벤트 감지 시에, 내부적으로 O(1)의 시간복잡도로 동작하여 select()에 비해 성능이 좋아, kqueue()를 사용했습니다.
3. 단일 프로세스에서 여러 I/O 작업을 동시에 처리할 수 있어, 리소스를 효율적으로 사용할 수 있습니다.
4. non-blocking이므로 I/O 작업 중 발생하는 대기시간을 최소화하여 응답속도를 높일 수 있습니다.

# Keep-alive
1. HTTP 1.1 규약에 따라 keep-alive를 지원하기 위해 kqueue에 Timer 이벤트를 등록하여 일정 시간 이상 client의 요청이 오지 않았을 때만 연결을 끊습니다.
2. HTTP 요청 헤더의 connection: close인 경우에는 Timer 이벤트를 등록하지 않고 HTTP 응답을 보내고 곧바로 연결을 끊습니다.

# Recive: request 객체
1. 클라이언트 소켓으로부터 HTTP 요청이 들어온 경우, `HttpRequestHandler::handle` 함수에서 처리합니다.
2. **소켓으로부터 요청을 읽어들여 각 소켓 당 마련된 버퍼에 저장**하고, keep-alive에 의해 버퍼에 여러 개의 요청이 쌓여 있을 수 있음을 전제하여 처리합니다.
3. `HttpRequestFactory::create` 함수에서 **하나의 HTTP 요청에 대응하는 하나의 HttpRequest 객체를 생성**합니다.        
4. 요청 메시지를 파싱하고, **chunked 요청인지 또는 일반 요청인지 파악하여 서로 다른 처리 로직이 구성**됩니다. HttpRequest 객체를 만들어 바로 반환하는 일반 요청과 달리, chunked 요청은 본문이 여러 번에 걸쳐 들어올 수 있음을 고려합니다.
6. 요청 메시지를 파싱하는 도중, 요청이 유효하지 않음을 발견하면 예외를 던져 클라이언트에게 오류 응답을 합니다.
7. 만들어진 유효한 HttpRequest 객체는 Send 쪽으로 전달됩니다.

# Send: response 객체
1. 전달 받은 request객체와 설정파일(conf)를 통해 client의 요청에 맞는 자원을 검색 및 반환합니다.
2. 요청한 파일이 정적 파일인 경우, response body에 담아 반환하고 동적 파일인 경우, 적합한 CGI script를 실행하여 결과를 반환합니다.
3. I/O multiplexing을 위해 socket fd와 fd에 kqueue에 등록하고, 이벤트가 발생했을 때 non-blocking으로 I/O 작업을 진행합니다.

# CGI script
동적 파일을 생성하거나, 요청 데이터를 서버에 전달하려고 할 때 CGI script를 사용합니다.
CGI 규약을 지켜 작성된 파일로 이번 프로젝트에서는 query string을 포함한 GET 요청, POST 요청, DELETE 요청을 처리하기 위한 용도로 작성되었습니다.
