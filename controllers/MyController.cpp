#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::~MyController()
{}

std::string	MyController::doExecute(HttpRequest &request, std::string data, const char *cgi_python)
{
	if (data == "")
		return ("");
	if (request.getMethod() == "POST" && request.getHeader("Content-Type") == "multipart/form-data")
		return (doExecuteLarge(data, cgi_python));
	return (doExecuteSmall(data, cgi_python));
}

// post
std::string    MyController::doExecuteLarge(std::string &data, const char *cgi_python)
{
	int		ret;
	char	buffer[64 * K];
	int     pipefd[2];
	const char    *path[4];
	std::string	saved_dir;

	// saved_dir : cgi_pthyon의 결과값을 저장하는 경로
	
	// write: 데이터를 갱신하자.

	path[0] = "/usr/bin/python3";
	path[1] = cgi_python;
	path[2] = saved_dir.c_str();
	path[3] = NULL;
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

// get, delete
std::string    MyController::doExecuteSmall(std::string &data, const char *cgi_python)
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
	std::string	bodyLength;
	std::string	cgiFile;
	std::stringstream ss;
	std::string	tmp[2];
	std::string	data;

	cgiFile = "cgi-bin/DoGet.py";
	tmp[0] = request.getParameter("username") == "" ? "" : "username=" + request.getParameter("username");
	tmp[1] = request.getParameter("password") == "" ? "" : "password=" + request.getParameter("password");
	data = tmp[0] + tmp[1];
	if (tmp[0] != "" && tmp[1] != "")
		data = tmp[0] + "&" + tmp[1];
	body = doExecute(request, data, cgiFile.c_str());
	response.ResponseStatusLine();
	response.putHeader("Content-Type", "text/html;charset=utf-8");
	ss << body.length();
	bodyLength = ss.str();
	response.putHeader("Content-Length", bodyLength);
	response.sendBody(body);
}

// delete
void	MyController::doDelete(HttpRequest &request, HttpResponse &response)
{
	std::string currentDate = getCurrentDate();
	std::string path;
	std::string	fileName = HttpConfig::pathResolver(request.getPath());
	std::string	cgiFile;
	std::string	body;

	if (access(fileName.c_str(), F_OK) == -1) // 존재하지 않는다.
		throw "404";
	cgiFile = "cgi-bin/DoDelete.py";
	body = doExecute(request, fileName, cgiFile.c_str());
	response.setStatusCode("200"); // if the action has been enacted and no further information is to be supplied.
	response.ResponseStatusLine();
	response.putHeader("Server", HttpConfig::getServerName());
	response.putHeader("Date", getCurrentDate());
	response.sendBody(body);
}

// file post
void	MyController::doPost(HttpRequest &request, HttpResponse &response)
{
	std::string			data;
	std::string			cgiFile;
	HttpRequestReader	reader(response.getSockfd());
	std::string			tmp;

	// cgiFile = HttpConfig::getCgiAddress(request.getMethod());
	cgiFile = "cgi-bin/DoUpload.py";
	tmp = "";
	while (request.getHeader("Transfer-Encoding") == "chunked"  && tmp != "0")
	{
		tmp = reader.getLine(); // 임시 파일 생성
		doExecute(request, tmp, cgiFile.c_str());
		if (tmp == "0") // -> 임시 파일 모두 삭제 후 하나의 파일로 merged
			reader.getLine(); // \r\n remove
	}
	if (request.getHeader("Transfer-Encoding") != "chunked")
	{
		doExecute(request, request.getQueryString(), cgiFile.c_str());
		doExecute(request, "0", cgiFile.c_str());
	}
	response.ResponseStatusLine();
	response.putHeader("Server", HttpConfig::getServerName());
	response.putHeader("Date", getCurrentDate());
	response.sendBody("");
}

std::string MyController::getCurrentDate() {
	// 현재 시각 얻기
	std::time_t currentTime_t;
    std::time(&currentTime_t);

    // 시각을 현지 시간대로 변환
    std::tm* localTime = std::localtime(&currentTime_t);

	// 현재 날짜 및 시간 문자열 생성
	std::ostringstream oss;
	oss << std::put_time(localTime, "%a, %d %b %Y %H:%M:%S GMT");
	return oss.str();
}