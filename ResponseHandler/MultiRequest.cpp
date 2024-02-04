#include"MultiRequest.hpp"

MultiRequest::MultiRequest(std::string contentType)
{
	std::string multipart;

	multipart = "multipart/form-data;";
	if (contentType.find(multipart) == std::string::npos)
		this->isMultipart = false;
	else
	{
		this->isMultipart = true;
		this->boundary = contentType.substr(contentType.find("boundary=") + std::string("boundary=").length());
	}
}
// cgi에서 contentDisposition를 분해해서 filename을 뽑자.
std::vector<HttpRequest>	*MultiRequest::makeRequest(HttpRequest request)
{
	size_t			next;
	size_t			cur;
	std::vector<HttpRequest>* requestVec;
	std::string		body;

	requestVec = new std::vector<HttpRequest>();
	if (this->isMultipart == false)
		requestVec->push_back(request);
	else
	{
		body = request.getBody();
		cur = this->boundary.length() + std::string("\r\n").length(); // 이상
		next = body.find(this->boundary, cur); // 미만
		while (next != std::string::npos)
		{
			HttpRequest	child;
			
			child.setHeader("Host", request.getHeader("Host"));
			child.setMethod(request.getMethod());
			
			fillEachRequest(child, body.substr(cur, next)); // [cur, next)

			std::cout << "=============== [Request line] ===============\n";
			std::cout << "method: " << child.getMethod() << '\n';
			std::cout << "path: " << child.getPath() << '\n';
			std::cout << "query string: " << child.getQueryString() << '\n';
		
			std::cout << "=============== [Request Header] ==============\n";
			child.printAllHeader();

			std::cout << "=============== [Request Body] ===============\n";
			std::cout << child.getBody() << "\n";



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
	request.printAllHeader();
	std::cout << "================================================\n";
}

MultiRequest::MultiRequest()
{}

MultiRequest::~MultiRequest()
{}
