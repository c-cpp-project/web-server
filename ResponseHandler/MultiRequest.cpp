#include"MultiRequest.hpp"

bool    MultiRequest::isMultipart(std::string contentType)
{
	std::string multipart;

	multipart = "MULTIPART/FORM-DATA;";
	if (multipart.compare(0, multipart.length(), contentType) == 0)
		return (true);
	return (false);
}

MultiRequest::MultiRequest(HttpRequest request)
{
	std::string tmp;
	size_t		i;
	std::string	body;

	tmp = request.getHeader("CONTENT-TYPE");
	i = tmp.find("boundary=");
	this->boundary = tmp.substr(i);
	this->count = 0;
	i = 0;
	// body = request.getRequestBody();
	body = HttpConfig::testBody;
	while (std::string::npos != body.find(this->boundary, i))
	{
		i += this->boundary.length();
		this->count+=1;
	}
	this->count-=1;
}

HttpRequest			*MultiRequest::makeRequest(HttpRequest request)
{
	HttpRequest	*dump;
	int			i;

	dump = new HttpRequest[this->count]();
	i = 0;
	while (i < this->count)
	{
		dump->setHeader("METHOD", request.getMethod());
		

	}
	return dump;
}

int	MultiRequest::getRequestCount()
{
	return (this->count);
}

MultiRequest::MultiRequest()
{}

MultiRequest::~MultiRequest()
{}
