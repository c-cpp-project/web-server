#include"Controller.hpp"

void			Controller::classifyEvent(std::string data, std::string contentType, const char *cgi_python, HttpResponse &response)
{
	int			ret;
	char		buffer[64 * K];
	int     	pipefd1[2];
	int			pipefd2[2];
	const char	*path[5];

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	pipe(pipefd2);
	if (contentType != "") // post
	{
		path[2] = response.getServerConfiguration()->getUploadPath().c_str();
		path[3] = contentType.c_str();
		path[4] = NULL;
		pipe(pipefd1);
	}
	else // get, delete 
	{
		path[2] = data.c_str();
		path[3] = NULL;
	}
	ret = fork();
	if (ret == 0)
	{
		close(pipefd2[0]); dup2(pipefd2[1], STDOUT_FILENO); // 출력
		close(pipefd2[1]);
		if (contentType != "")
		{
			close(pipefd1[1]); dup2(pipefd1[0], STDIN_FILENO); // 입력
			close(pipefd1[0]);
		}
		execve("/usr/bin/python3", const_cast<char* const*>(path), NULL);
	}
	else
	{
		if (contentType != "")
			writeEventRegister(pipefd1, pipefd2, response, data);
		else
			readEventRegsiter(pipefd2, response);
	}
}

void			Controller::writeEventRegister(int writefd[2], int readfd[2], HttpResponse &response, std::string data)
{
	ServerConfiguration *serverConfig;
	Event				*event;

	serverConfig = response.getServerConfiguration();
	event = response.getEvent();
	close(writefd[0]);
	fcntl(writefd[1], F_SETFL, O_NONBLOCK);
	BeanFactory::registerEvent("WRITE", new HttpHandler(writefd[1], data, serverConfig), event);
	readEventRegsiter(readfd, response);
}

// request
// response
void			Controller::readEventRegsiter(int readfd[2], HttpResponse &response)
{
	ServerConfiguration *serverConfig;
	Event				*event;

	serverConfig = response.getServerConfiguration();
	event = response.getEvent();
	close(readfd[1]);
	fcntl(readfd[0], F_SETFL, O_NONBLOCK);
	BeanFactory::registerEvent("READ", new HttpHandler(readfd[0], &response), event);
}

// parameter
void    Controller::doGet(HttpRequest &request, HttpResponse &response)
{
	std::string	body;
	std::string	cgiFile;
	std::string	tmp[2];
	std::string	data;

	cgiFile = "cgi-bin/DoGet.py";
	tmp[0] = request.getParameter("username") == "" ? "" : "username=" + request.getParameter("username");
	tmp[1] = request.getParameter("password") == "" ? "" : "password=" + request.getParameter("password");
	data = tmp[0] + tmp[1];
	if (tmp[0] != "" && tmp[1] != "")
		data = tmp[0] + "&" + tmp[1];
	classifyEvent(data, "", cgiFile.c_str(), response);
}

// file post
void	Controller::doPost(HttpRequest &request, HttpResponse &response)
{
	std::string	data;
	std::string	cgiFile;
	std::string	contentType;
	std::string	body;

	cgiFile = "cgi-bin/DoUpload.py";
	data = request.getBody();
	contentType = request.getHeader("content-type");
	classifyEvent(data, contentType, cgiFile.c_str(), response);
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string	fileName;
	std::string	cgiFile;
	std::string	body;
	std::string	uri;

	cgiFile = "cgi-bin/DoDelete.py";
	uri = request.getPath();
	fileName = uri.substr(uri.find('/', 1) + 1);
	if (access(fileName.c_str(), F_OK) == -1) // 존재하지 않는다.
		throw "404";
	classifyEvent(fileName, "", cgiFile.c_str(), response);
}

Controller::Controller()
{
	this->masking = 1;
}

Controller::Controller(int masking)
{
	this->masking = masking;
}

Controller::~Controller()
{}

bool    Controller::isAcceptableMethod(std::string method)
{
	if (method == "GET" && (METHOD::GET & this->masking))
		return (true);
	else if (method == "POST" && (METHOD::POST & this->masking))
		return (true);
	else if (method == "DELETE" && (METHOD::DELETE & this->masking))
		return (true);
	else
		return (false);
}