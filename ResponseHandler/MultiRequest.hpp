#ifndef MULTI_REQUEST_HPP
# define MULTI_REQUEST_HPP

#include<string>
#include<vector>
#include <sstream>
#include"../HttpRequest/HttpRequest.hpp"
#include"../ResponseConfig.hpp"
# define NDEBUG 
#include <assert.h>

# define INCOMPLETE_REQUEST 1 // 요청을 더 읽어오자
# define START_CHUNKED_REQUEST 2 // 청크 전송을 시작하는 요청
# define BAD_REQUEST 400 // 400 응답으로 현재 요청을 거부하자
# define LENGTH_REQUIRED 411 // 411 응답으로 현재 요청을 거부하자

class HttpRequest;
class MultiRequest
{
private:
	std::string boundary;
	HttpRequest	request;
	bool		isMultipart;
public:
	std::vector<HttpRequest>	*makeRequest(HttpRequest request); // CONTENT-TYPE 변경 및 CONTENT-LENGTH 추가 -> 생성 반환
	void			fillEachRequest(HttpRequest &request, std::string body);
	MultiRequest();
	MultiRequest(std::string contentType);
	~MultiRequest();
};

#endif

// POST /example-endpoint HTTP/1.1
// Host: example.com
// Content-Type: multipart/form-data; boundary=---------------------------1234567890123456789012345678

// -----------------------------1234567890123456789012345678
// Content-Disposition: form-data; name="text_field"

// This is a simple text field.
// -----------------------------1234567890123456789012345678
// Content-Disposition: form-data; name="file"; filename="example.txt"
// Content-Type: text/plain

// Contents of the file go here.
// -----------------------------1234567890123456789012345678--

// requst를 multipart의 개수만큼 생성하여 buffer에 쌓는다.
// 전송은 한번에 한다.
// 응답은 200 OK를 body에 담아서 보낸다.
