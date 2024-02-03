#ifndef MULTI_REQUEST_HPP
# define MULTI_REQUEST_HPP

#include<string>
#include"../HttpRequest/HttpRequest.hpp"
#include"../HttpConfig.hpp"

class HttpRequest;
class MultiRequest
{
private:
	std::string boundary;
	int			count;
public:
	static	bool	isMultipart(std::string contentType); // Content-Type: multipart/form-data; 확인
	HttpRequest			*makeRequest(HttpRequest request); // CONTENT-TYPE 변경 및 CONTENT-LENGTH 추가 -> 생성 반환
	
	int			 	getRequestCount();
	MultiRequest(HttpRequest request);
	MultiRequest(std::string body);
	MultiRequest();
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
