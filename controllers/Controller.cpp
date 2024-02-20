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
	std::cout << "[" << contentType << " : contentType]\n";
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
		// wait(NULL);
	}
}

void			Controller::writeEventRegister(int writefd[2], int readfd[2], HttpResponse &response, std::string data)
{
	ServerConfiguration *serverConfig;
	Event				*event;
	HttpHandler			*handler;

	serverConfig = response.getServerConfiguration();
	event = response.getEvent();
	handler = new HttpHandler(writefd[1], data, serverConfig);
	close(writefd[0]);
	fcntl(writefd[1], F_SETFL, O_NONBLOCK);
	event->saveEvent(writefd[1], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, handler);
	// event->saveEvent(response.getSockfd(), EVFILT_READ, 0, 0, 0, new HttpHandler(response.getSockfd(), response)); // EVFILT_READ, EVFILT_WRITE
	readEventRegsiter(readfd, response);
}

// request
// response
void			Controller::readEventRegsiter(int readfd[2], HttpResponse &response)
{
	Event				*event;

	event = response.getEvent();
	close(readfd[1]);
	fcntl(readfd[0], F_SETFL, O_NONBLOCK);
	event->saveEvent(readfd[0], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(readfd[0], response)); // EVFILT_READ, EVFILT_WRITE
	// BeanFactory::registerEvent("READ", new HttpHandler(readfd[0], response), event);
}

// parameter
void    Controller::doGet(HttpRequest &request, HttpResponse &response)
{
	std::string	body;
	std::string	cgiFile;
	std::string	tmp[2];
	std::string	data;

	std::cout << "Controller::doGet\n";
	// cgiFile = "cgi-bin/DoGet.py";
	cgiFile = response.getServerConfiguration()->getGetCgiPath();
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

	std::cout << "Controller::doPost\n";
	// cgiFile = "cgi-bin/DoUpload.py";
	cgiFile = response.getServerConfiguration()->getPostCgiPath();
	data = request.getBody();
	contentType = request.getHeader("Content-Type");
	classifyEvent(data, contentType, cgiFile.c_str(), response);
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string	fileName;
	std::string	cgiFile;
	std::string	body;
	std::string	uri;

	// cgiFile = "cgi-bin/DoDelete.py";
	cgiFile = response.getServerConfiguration()->getDeleteCgiPath();
	fileName = request.getPath();
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

Controller::Controller(int masking, std::string mLocation)
{
	this->masking = masking;
	this->mLocation = mLocation;
}

Controller::~Controller()
{}

bool    Controller::isAcceptableMethod(std::string method)
{
	std::cout << "this->masking : " << this->masking << ", " << method << "\n";
	if (method == "GET" && (1 & this->masking))
		return (true);
	else if (method == "POST" && (2 & this->masking))
		return (true);
	else if (method == "DELETE" && (8 & this->masking))
		return (true);
	else
		return (false);
}
