#include"MultiRequest.hpp"

MultiRequest::MultiRequest(HttpRequest request)
{
	std::string tmp;
	size_t		i;
	std::string multipart;

	multipart = "multipart/form-data;";
	if (multipart.compare(0, multipart.length(), request.getHeader("Content-Type")) != 0)
		this->isMultipart = false;
	else
	{
		tmp = request.getHeader("Content-Type");
		this->boundary = tmp.substr(tmp.find("boundary="));
	}
	this->request = request;
}

// cgi에서 contentDisposition를 분해해서 filename을 뽑자.
std::vector<HttpRequest>	*MultiRequest::makeRequest()
{
	size_t			next;
	size_t			cur;
	std::vector<HttpRequest>* requestVec;
	std::string		body;

	requestVec = new std::vector<HttpRequest>();
	if (this->isMultipart == false)
		requestVec->push_back(this->request);
	else
	{
		body = request.getBody();
		cur = this->boundary.length() + std::string("\r\n").length(); // 이상
		next = body.find(this->boundary, cur); // 미만
		while (next != std::string::npos)
		{
			HttpRequest	child;
			
			child.setHeader("Host", request.getHeader("Host"));
			child.setHeader("Host", request.getHeader("Host"));
			fillEachRequest(child, body.substr(cur, next)); // [cur, next)
			requestVec->push_back(child);
			cur = next + this->boundary.length() + std::string("\r\n").length();
			next = body.find(this->boundary, cur);
		}
	}
	return requestVec;
}

void			MultiRequest::fillEachRequest(HttpRequest &request, std::string body)
{
	size_t				cur;
	size_t				next;
	std::stringstream 	ss;

	cur = 0;
	while (true) // header
	{
		next = body.find("\r\n", cur);
		if (cur == next)
			break ;
		std::string line = body.substr(cur, next - cur);
		request.addHeader(line);
		cur += std::string("\r\n").length();
	}
	cur += std::string("\r\n").length(); // body 시작 index
	ss << (body.length() - cur);
	request.setHeader("Content-Length", ss.str());
	request.setRequestBody(body.substr(cur));
	assert(ss.str() == body.substr(cur).length());
}

MultiRequest::MultiRequest()
{}

MultiRequest::~MultiRequest()
{}
