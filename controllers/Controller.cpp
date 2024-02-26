#include"Controller.hpp"

void			Controller::classifyEvent(HttpRequest &request, HttpResponse &response, const char *cgi_python, std::string filename)
{
	int			ret;
	char		buffer[64 * K];
	int     	pipefd1[2];
	int			pipefd2[2];
	const char	*path[6];
	std::string uploadPath = request.getPath();
	std::string	queryString;

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	pipe(pipefd2);
	if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded")
	{
		queryString = request.getMethod() == "GET" ? request.getQueryString() : request.getBody();

		path[2] = uploadPath.c_str();
		path[3] = queryString.c_str();
		path[4] = filename.c_str();
		std::cout << "ARGV: [" << path[2] << ", " << path[3] << ", " << path[4] << "]\n";
		path[5] = NULL;
	}
	else if (request.getMethod() == "POST") // post
	{
		path[2] = uploadPath.c_str();
		path[3] = filename.c_str(); // filename
		pipe(pipefd1);
		std::cout << "ARGV: [" << path[2] << ", " << path[3] << "]\n";
		path[4] = NULL;
	}
	else
	{
		path[2] = filename.c_str(); // target
		std::cout << "ARGV: [" << path[2] << "]\n";
		path[3] = NULL;
	}
	std::cout << request.getMethod() << "[" << filename << " : contentType]\n";
	ret = fork();
	if (ret == 0)
	{
		close(pipefd2[0]); dup2(pipefd2[1], STDOUT_FILENO); // 출력
		close(pipefd2[1]);
		if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded")
		{
			close(pipefd1[1]); dup2(pipefd1[0], STDIN_FILENO); // 입력
			close(pipefd1[0]);
		}
		execve("/usr/bin/python3", const_cast<char* const*>(path), NULL);
	}
	else
	{
		ChildProcess::insertChildProcess(ret);
		if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded")
			writeEventRegister(pipefd1, pipefd2, response, request.getBody());
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
}

std::pair<std::string, std::string>	Controller::getFileName(HttpRequest &request)
{
	std::string		filename = "";
	std::string		directory = "";
	std::string		fullpath = request.getPath();
	struct stat		buf;
	struct dirent	*entry;
	size_t			s, e;

	stat(fullpath.c_str(), &buf);
	if (S_ISDIR(buf.st_mode))
	{
		if (request.getHeader("CONTENT-DISPOSITION") != "")
		{
			filename = request.getHeader("CONTENT-DISPOSITION").substr(request.getHeader("CONTENT-DISPOSITION").find("filename="));
			s = filename.find('\"');
			e = filename.find('\"', s + 1);
			filename = filename.substr(s + 1, e - s - 1);
		}
		else
		{
			DIR		*dir;
			size_t	count;
			std::stringstream ss;

			count = 0;
			dir = opendir(fullpath.c_str());
			while ((entry = readdir(dir)) != NULL)
				count++;
			ss << count;
			filename = ss.str();
		}
		if (filename.find(".") == std::string::npos)
			filename = filename + "." + request.getHeader("content-Type").substr(request.getHeader("content-Type").find("/") + 1);
		std::cout << "doPost file_name: ["<< filename << "]\n";
		directory = fullpath;
	}
	else
	{
		int	i;

		for (i = fullpath.length() - 1; i >= 0; i--)
		{
			if (fullpath[i] == '/')
				break ;
		}
		directory = fullpath.substr(0, i);
		filename = fullpath.substr(i + 1);
	}
	return (std::pair<std::string, std::string>(directory, filename));
}

// parameter
void    Controller::doGet(HttpRequest &request, HttpResponse &response)
{
	std::string	cgiFile;
	std::string filename;
	std::pair<std::string, std::string>	path;

	cgiFile = response.getServerConfiguration()->getGetCgiPath();
	path = getFileName(request);
	request.setPath(path.first);
	filename = path.second;
	classifyEvent(request, response, cgiFile.c_str(), filename);
}

// file post
void	Controller::doPost(HttpRequest &request, HttpResponse &response)
{
	std::string		cgiFile;
	std::string 	filename;
	std::pair<std::string, std::string>	path;

	std::cout << request.getBody().length() << " = Controller::doPost\n";
	cgiFile = response.getServerConfiguration()->getGetCgiPath();
	path = getFileName(request);
	request.setPath(path.first);
	filename = path.second;
	classifyEvent(request, response, cgiFile.c_str(), filename);
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string	target;
	std::string	cgiFile;

	cgiFile = response.getServerConfiguration()->getDeleteCgiPath();
	target = request.getPath();
	classifyEvent(request, response, cgiFile.c_str(), target);
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
	if ((method == "GET")&& (1 & this->masking))
		return (true);
	// if ((method == "GET")&& (1 & this->masking))
	// 	return (true);
	else if (method == "POST" && (2 & this->masking))
		return (true);
	else if (method == "DELETE" && (8 & this->masking))
		return (true);
	else
		return (false);
}
