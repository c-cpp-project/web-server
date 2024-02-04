#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::~MyController()
{}

// post
std::string    MyController::doExecuteWrite(std::string &data, std::string filename, const char *cgi_python)
{
	int			ret;
	char		buffer[64 * K];
	int     	pipefd[2];
	const char	*path[5];
	std::string	saved_dir;

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	path[2] = saved_dir.c_str();
	path[3] = filename.c_str();
	path[4] = NULL;
	pipe(pipefd);
	ret = fork();
	if (ret == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (execve("/usr/bin/python3", const_cast<char* const*>(path), NULL) < 0)
			throw "500";
	}
	else
	{
		close(pipefd[0]);
		// fcntl(pipefd1[1], F_SETFL, O_NONBLOCK);

		// doExecuteLarge에서도 출력값을 전달 받아야 한다.
		// 그렇게 해야 response message를 생성한다.
		write(pipefd[1], data.c_str(), data.size());
		close(pipefd[1]);
		wait(NULL);
		if (ret < 0)
			throw ("500");
	}
	buffer[ret] = 0;
	return (buffer);
}

// get
std::string    MyController::doExecuteRead(std::string &data, const char *cgi_python)
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

// parameter
void    MyController::doGet(HttpRequest &request, HttpResponse &response)
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
void	MyController::doPost(HttpRequest &request, HttpResponse &response)
{
	std::string	data;
	std::string	cgiFile;
	std::string	contentType;
	std::string	body;

	cgiFile = "cgi-bin/DoPost.py";
	data = request.getBody();
	contentType = request.getHeader("content-type");
	body = doExecuteWrite(data, contentType, cgiFile.c_str());
	if (body == "500")
		throw "500";
	response200(body, response);
}