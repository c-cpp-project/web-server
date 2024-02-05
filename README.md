# kqueue

### kqueue 3가지
	1. write -> cgi write => pipefd1[1] event -> write(pipefd1[1], data.c_str(), data.length());
	2. read -> response => pipefd2[0] event -> ret = read(pipefd2[0], buffer, 64 * K); -> response
    3. socket -> request


### 이벤트 등록

구분해서 처리 (read, write) => std::map

### 서버 설정

해당 서버 설정 - static을 풀고 HttpRequestHandler 풀고 ServerConfiguration 필드로 추가하고 생성자에서 매개변수로 받는 방식으로 바꾸기.

# Request

1. HttpRequestHandler에서 멤버변수들 static 풀기
2. ServerConfiguration을 멤버변수로 추가하고, socket_fd랑 같이 생성자에서 매개변수로 받기

# Response

1. Delete: controller에 저장하는 방식으로 사용하기. [기존 방식으로 수정]
2. request-range하기
3. pipe에 대한 NON_BLOCK


