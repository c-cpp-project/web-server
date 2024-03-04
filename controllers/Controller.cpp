#include"Controller.hpp"

void			Controller::classifyEvent(HttpRequest &request, HttpResponse &response, const char *cgi_python)
{
	int			pid;
	int     	pipefd1[2];
	int			pipefd2[2];
	char const 	*argv[3];
	std::string	data;

	request.setHeader("SCRIPT-NAME", std::string(cgi_python));
	if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded")
		pipe(pipefd1);
	pipe(pipefd2);
	pid = fork();
	if (pid == 0)
	{
		char	 	**envp;

		envp = envpList(request);
		close(pipefd2[0]); dup2(pipefd2[1], STDOUT_FILENO); // 출력
		close(pipefd2[1]);
		if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded")
		{
			close(pipefd1[1]); dup2(pipefd1[0], STDIN_FILENO); // 입력
			close(pipefd1[0]);
		}
		if (strcmp(cgi_python, "cgi-bin/cgi_tester") == 0)
		{
			argv[0] = cgi_python;
			argv[1] = NULL;
		}
		else
		{
			argv[0] = "/usr/bin/python3";
			argv[1] = cgi_python;
		}
		argv[2] = NULL;
		execve(argv[0], const_cast<char* const*>(argv), envp);
		freeEnvpList(envp);
	}
	else
	{
		ChildProcess::insertChildProcess(pid);
		if (request.getMethod() == "POST" && request.getHeader("CONTENT-TYPE") != "application/x-www-form-urlencoded")
			writeEventRegister(pipefd1, pipefd2, response, request.getBody());
		else
		{
			if (request.getQueryString() != "")
				readEventRegsiter(pipefd2, response, request.getQueryString().length());
			else
				readEventRegsiter(pipefd2, response, request.getBody().length());
		}
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
	std::cout << writefd[1] << " = Controller::readEventRegsiter\n";
	event->saveEvent(writefd[1], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,  new HttpHandler(writefd[1], data, serverConfig));
	readEventRegsiter(readfd, response, data.length());
}

void			Controller::readEventRegsiter(int readfd[2], HttpResponse &response, size_t bodySize)
{
	Event				*event;

	event = response.getEvent();
	close(readfd[1]);
	fcntl(readfd[0], F_SETFL, O_NONBLOCK);
	std::cout << readfd[0] << " = Controller::readEventRegsiter\n";
	std::cout << bodySize << " = bodySize\n";
	event->saveEvent(readfd[0], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, new HttpHandler(readfd[0], response, 0)); // EVFILT_READ, EVFILT_WRITE
}

std::string	Controller::changeToUnderbar(std::string src)
{
	for (int i = 0; i < src.length(); i++)
	{
		if (src[i] == '-')
			src[i] = '_';
	}
	return (src);
}

char		**Controller::envpList(HttpRequest &request)
{
	std::map<std::string, std::string>::iterator iter;
	size_t		size = 0;
	size_t		idx = 0;
	char	 	**envp;

	for (iter = request.getHeaderBegin(); iter != request.getHeaderEnd(); iter++)
		size++;
	size += 9; // METHOD, SERVER_PROTOCOL, PATH_INFO, REQUEST_URI, CONTENT_TYPE, PATH_TRANSLATED; SERVER_PROTOCOL; SERVER_SOFTWARE, GATEWAY_INTERFACE
	if (request.getQueryString() != "")
		size++; // Query String
	envp = new char*[size + 1];
	envp[idx++] = strdup(("REQUEST_METHOD=" + request.getMethod()).c_str());
	if (request.getQueryString() != "")
		envp[idx++] = strdup(("QUERY_STRING=" + request.getQueryString()).c_str());
	envp[idx++] = strdup(("PATH_INFO=" + request.getRepository()).c_str());
	envp[idx++] = strdup(("PATH_TRANSLATED=" + request.getPath()).c_str());
	envp[idx++] = strdup(("REQUEST_URI=" + request.getRepository()).c_str());
	envp[idx++] = strdup(("CONTENT_TYPE=" + ResponseConfig::getContentType(request.getPath())).c_str());
	envp[idx++] = strdup(("SERVER_PROTOCOL=" + request.getProtocolString()).c_str());
	envp[idx++] = strdup(std::string("SERVER_SOFTWARE=NGINX").c_str());
	envp[idx++] = strdup(std::string("GATEWAY_INTERFACE=CGI/1.1").c_str());
	for (iter = request.getHeaderBegin(); iter != request.getHeaderEnd(); iter++)
	{
		std::string	key;
		std::string	value;
		std::string	env;

		key = iter->first;
		if (iter->first.find("ACCEPT") != std::string::npos || iter->first.find("CONTENT-TYPE") != std::string::npos)
			continue;
		if (iter->first.find("SEC-") != std::string::npos)
			continue;
		if (iter->first.find("CONTENT-LENGTH") == std::string::npos && iter->first.find("SERVER-PROTOCOL") == std::string::npos \
		&& iter->first.find("SERVER_PORT") == std::string::npos && iter->first.find("SCRIPT-NAME") == std::string::npos)
			key = "HTTP_" + changeToUnderbar(key);
		else
			key = changeToUnderbar(key);
		value = changeToUnderbar(iter->second);
		env = key + "=" + value;
		envp[idx++] = strdup(env.c_str());
	}
	envp[idx] = 0;
	for (int i = 0; envp[i] != 0; i++)
	{
		std::cout << envp[i] << "\n";
	}
	return (envp);
}

void	Controller::freeEnvpList(char *const *envp) {
    for (size_t i = 0; envp[i] != nullptr; ++i) {
        free(envp[i]);
    }
    delete[] envp;
}

std::pair<std::string, std::string>	Controller::getFileName(HttpRequest &request)
{
	std::string		filename = "";
	std::string		directory = "";
	std::string		fullpath = request.getPath();
	std::string		type = "";
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
			closedir(dir);
			ss << count;
			filename = ss.str();
		}
		if (filename.find(".") == std::string::npos)
		{
			type = request.getHeader("content-Type").substr(request.getHeader("content-Type").find("/") + 1);
			if (type == "")
				type = "txt";
			filename = filename + "." + type;
		}
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
	if (request.getQueryString() == "")
		request.setQueryString(request.getBody());
	request.setPath(path.first + "/" + path.second);
	classifyEvent(request, response, cgiFile.c_str());
}

// file post
void	Controller::doPost(HttpRequest &request, HttpResponse &response)
{
	std::string		cgiFile;
	std::string 	filename;
	std::pair<std::string, std::string>	path;

	std::cout << request.getBody().length() << " = Controller::doPost\n";
	cgiFile = response.getServerConfiguration()->getPostCgiPath();
	path = getFileName(request);
	request.setPath(path.first + "/" + path.second);
	classifyEvent(request, response, cgiFile.c_str());
}

void	Controller::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string	target;
	std::string	cgiFile;

	cgiFile = response.getServerConfiguration()->getDeleteCgiPath();
	target = request.getPath();
	classifyEvent(request, response, cgiFile.c_str());
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
	else if (method == "POST" && (2 & this->masking))
		return (true);
	else if (method == "DELETE" && (8 & this->masking))
		return (true);
	else
		return (false);
}
