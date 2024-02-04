
#include"DeleteController.hpp"
// delete

DeleteController::DeleteController()
{}

DeleteController::~DeleteController()
{}

std::string    DeleteController::doExecute(HttpRequest &request, std::string data, const char *cgi_python)
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

void	DeleteController::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string currentDate = getCurrentDate();
	std::string	fileName;
	std::string	cgiFile;
	std::string	body;
	std::stringstream ss;
	std::string	bodyLength;

	fileName = request.getPath().substr(1);
	if (access(fileName.c_str(), F_OK) == -1) // 존재하지 않는다.
		throw "404";
	cgiFile = "cgi-bin/DoDelete.py";
	body = doExecute(request, fileName, cgiFile.c_str());
	if (body == "500")
		throw "500";
	response200(body, response);
}