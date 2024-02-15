#include"Controller.hpp"

// post
std::string    Controller::doExecuteWrite(std::string &data, std::string contentType, const char *cgi_python)
{
	int			ret;
	char		buffer[64 * K];
	int     	pipefd1[2];
	int			pipefd2[2];
	const char	*path[5];
	std::string	saved_dir = "repository";

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	path[2] = saved_dir.c_str();
	path[3] = contentType.c_str();
	path[4] = NULL;
	pipe(pipefd1);
	pipe(pipefd2);
	ret = fork();
	if (ret == 0)
	{
		// 입력
		close(pipefd1[1]); dup2(pipefd1[0], STDIN_FILENO);
		close(pipefd1[0]);

		// 출력
		close(pipefd2[0]); dup2(pipefd2[1], STDOUT_FILENO);
		close(pipefd2[1]);
		if (execve("/usr/bin/python3", const_cast<char* const*>(path), NULL) < 0)
			throw "500";
	}
	else
	{
		// 입력
		close(pipefd1[0]);
		write(pipefd1[1], data.c_str(), data.length());
		close(pipefd1[1]);

		// 출력
		close(pipefd2[1]);
		ret = read(pipefd2[0], buffer, 64 * K);
		close(pipefd2[0]);
		// fcntl(pipefd1[1], F_SETFL, O_NONBLOCK);
		wait(NULL);
		if (ret < 0)
			throw ("500");
	}
	buffer[ret] = 0;
	std::cout << "buffer:[" << buffer << "]\n";
	return (buffer);
}

// request
// response

// get
std::string    Controller::doExecuteRead(std::string &data, const char *cgi_python)
{
	const char    *path[4];
	char	buffer[64 * K];
	int		fileOut[2];
	int		ret;

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	path[2] = data.c_str();
	path[3] = NULL;
	pipe(fileOut);
	ret = fork();
	if (ret == 0)
	{
		close(fileOut[0]);
		dup2(fileOut[1], STDOUT_FILENO); close(fileOut[1]);
		execve("/usr/bin/python3", const_cast<char* const*>(path), NULL);
	}
	else if (ret > 0)
	{
		close(fileOut[1]);
		// fcntl(pipefd2[0], F_SETFL, O_NONBLOCK);
		ret = read(fileOut[0], buffer, 64 * K);
		close(fileOut[0]);
	}
	if (ret < 0)
		throw "500";
	buffer[ret] = 0;
	return (buffer);
}

std::string    Controller::doExecuteDelete(HttpRequest &request, std::string data, const char *cgi_python)
{
	const char    *path[4];
	char	buffer[64 * K];
	int		fileOut[2];
	int		ret;

	request.getPath();
	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	path[2] = data.c_str();
	path[3] = NULL;
	pipe(fileOut);
	ret = fork();
	if (ret == 0)
	{
		close(fileOut[0]);
		dup2(fileOut[1], STDOUT_FILENO); close(fileOut[1]);
		execve("/usr/bin/python3", const_cast<char* const*>(path), NULL);
	}
	else if (ret > 0)
	{
		close(fileOut[1]);
		// fcntl(pipefd2[0], F_SETFL, O_NONBLOCK);
		ret = read(fileOut[0], buffer, 64 * K);
		close(fileOut[0]);
	}
	if (ret < 0)
		throw "500";
	buffer[ret] = 0;
	return (buffer);
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
	body = doExecuteRead(data, cgiFile.c_str());
	if (body == "500")
		throw "500";
	response200(body, response);
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
	body = doExecuteWrite(data, contentType, cgiFile.c_str());
	if (body == "500")
		throw "500";
	response200(body, response);
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
	body = doExecuteDelete(request, fileName, cgiFile.c_str());
	if (body == "500")
		throw "500";
	response200(body, response);
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

void	    Controller::response200(std::string body, HttpResponse &response)
{
	std::stringstream ss;
	std::string	bodyLength;

	response.putHeader("Server", ResponseConfig::getServerName());
	response.putHeader("Date", ResponseConfig::getCurrentDate());
	response.putHeader("Content-Type", "text/html;charset=utf-8");
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
	response.sendBody(body);
}

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