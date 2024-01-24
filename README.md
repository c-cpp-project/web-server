# web-server
c++로 Nginx와 같은 웹서버를 직접 구현했습니다.

### NGINX 역할
1. redirect
2. 정적 파일 서빙

### CGI 역할
1. POST/GET 처리 - parameter
2. delete 처리
3. POST 처리 - 파일 업로드

### FrontController.cpp
1. response.forward(request, response): 정적 파일과 실패.html
2. controller.service(requesst, response):  CGI 실행 및 redirect

### HttpConfig::pathResolver
std::map<std::string, std::string> umap;  
umap.put("uri", "path") 

# 문제 상황
### 어떻게 CGI를 구현할 것인가?

### 어떻게 chunked request를 CGI에서 구현할 것인가?