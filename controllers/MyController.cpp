#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::MyController(int masking) : Controller(masking)
{}

MyController::MyController(int masking, std::string mLocation) : Controller(masking, mLocation)
{}

MyController::~MyController()
{}

std::string	MyController::findFullPath(std::string fullpath, std::string page)
{
	DIR             *dir;
	struct dirent	*entry;
	size_t          idx;
	std::string     d_name;
	struct stat 	buf;
	int				i;

	if (fullpath[fullpath.length() - 1] == '/')
		fullpath = fullpath.substr(0, fullpath.length() - 1);
	for (i = fullpath.length() - 1; i >= 0; i--)
	{
		if (fullpath[i] == '/')
			break ;
	}
	std::string		directory = (i < 0) ? fullpath : fullpath.substr(0, i);
	std::string		file = (i < 0) ? "" : fullpath.substr(i + 1);

	std::cout << "MyController::findFullPath: ["<< directory << "], [" << file  << "]\n";
	dir = opendir(directory.c_str());
	if (dir == NULL || file == "")
	{
		std::cout << "Error Opening directory OR Only Directory\n";
		if (file == "" && directory != "")
			closedir(dir);
		throw "404";
	}
	// file에 확장자 없을 경우: 가장 처음으로 만나는 동일한 이름의 파일에 대응된다.
	while ((entry = readdir(dir)) != NULL)
	{
		d_name = std::string(entry->d_name);
		stat(std::string(directory + "/" + d_name).c_str(), &buf);
		if (S_ISDIR(buf.st_mode) && d_name == file)
		{
			directory = fullpath;
			d_name = page;
			break ;
		}
		else if ((file.find(".") == std::string::npos && d_name.substr(0, d_name.find(".")) == file) || file == d_name)
			break ;
	}
	closedir(dir);
	if (entry == NULL)
		throw "404";
	return (directory + "/" + d_name);
}

std::string	MyController::getLocationIndex(ServerConfiguration *serverConfig, std::string path)
{
	std::vector<std::string>	vec;
	
	vec = serverConfig->getLocation(path)->getIndex();
	if (vec.size() == 0)
		return ("");
	return (vec[0]);
}

// request.path를 수정해야 한다. 폴더/파일, 폴더
// request.setPath(목적지) -> 폴더
// 파일이 없다면 파일, 파일이 있다면 해당 파일명
void	MyController::runCgiScript(HttpRequest &request, HttpResponse &response)
{
	ServerConfiguration *serverConfig = response.getServerConfiguration();
	std::string 	index = getLocationIndex(serverConfig, request.getPath());
	std::string		root = serverConfig->getLocation(request.getPath())->getRoot();
	std::string		fullpath;
	std::string		targetPath;
	std::string		mainChain;
	std::string		request_uri = request.getPath();

	try
	{
		mainChain = serverConfig->findLocationUri(request.getPath());
		if (request.getPath() == mainChain)
			fullpath = root;
		else
			fullpath = std::string(root + "/" + request.getPath().substr(mainChain.length() + 1));
		targetPath = findFullPath(fullpath, "");
	}
	catch(const char *e)
	{
		std::cout << "NO FILE: " << e << "\n"; // DIRECTORY OR NO FILE
		if (access(fullpath.c_str(), F_OK) != 0) // BEING IS NOTING
		{
			int fd = open(fullpath.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
			close(fd);
			if (fd < 0)
				throw "500";
		}
		targetPath = fullpath;
	}
	// targetPath는 폴더 혹은 파일이다.
	if (targetPath[targetPath.length() - 1] == '/')
		targetPath = targetPath.substr(0, targetPath.length() - 1);
	request_uri += (request.getQueryString() == "" ? "" : "?" + request.getQueryString());
	request.setRepository(request_uri);
	request.setPath(targetPath);
	if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
		doGet(request, response);
	else if (request.getMethod() == "POST")
		doPost(request, response);
	else if (request.getMethod() == "DELETE")
		doDelete(request, response);
}

void	MyController::runService(HttpRequest &request, HttpResponse &response)
{
	ServerConfiguration *serverConfig = response.getServerConfiguration();
	Location			*location;
	std::string			redirectPath;
	std::string			staticPath;

	redirectPath = serverConfig->getRedirectionPath(request.getPath()).second;
	location = serverConfig->getLocation(request.getPath());
	if (location != NULL && location->getAutoIndex() && request.getPath() == serverConfig->findLocationUri(request.getPath()))
		response.listDirectory(location->getRoot());
	else if (redirectPath != "")
	{
		response.setStatusCode(serverConfig->getRedirectionPath(request.getPath()).first);
		request.setPath(redirectPath);
		response.redirect(request.getPath());
	}
	else
	{
		std::string index = getLocationIndex(serverConfig, request.getPath());
		std::string	root = location->getRoot();
		std::string	mainChain;

		if (root[root.length() - 1] == '/')
			root = root.substr(0, root.length() - 1);
		if (request.getPath() == serverConfig->findLocationUri(request.getPath())) // /root/index_file
			staticPath = findFullPath(std::string(root + "/" + index), index);
		else // /root/file_name
		{
			mainChain = serverConfig->findLocationUri(request.getPath());
			staticPath = findFullPath(std::string(root + "/" + request.getPath().substr(mainChain.length() + 1)), index);
		}
		std::cout << staticPath << " = staticPath\n";
		request.setPath(staticPath);
		response.forward(request);
	}
}

void    MyController::service(HttpRequest &request, HttpResponse &response)
{
	std::string 		cgiPath;
	ServerConfiguration *serverConfig = response.getServerConfiguration();
	std::stringstream 	ss;
	std::string			extension = serverConfig->getCgiTestExt();
	bool				allowedMethod;

	std::cout << "MyController::service" << "\n";
	allowedMethod = false;
	if (extension != "" && request.getPath().length() >= extension.length() && \
	request.getPath().substr(request.getPath().length() - extension.length()) == extension)
	{
		if (request.getMethod() == "POST")
			allowedMethod = true;
	}
	if (isAcceptableMethod(request.getMethod()) == false && allowedMethod == false)
		throw "405";
	ss << serverConfig->getPort();
	request.setHeader("SERVER_PORT", ss.str());
	request.setHeader("SERVER_SOFTWARE", serverConfig->getServerName());
	if (request.getHeader("connection") == "close")
		response.putHeader("connection", "close");
	else
		response.putHeader("connection", "");
	if ((request.getMethod() == "GET") && request.getQueryString() == "")
		runService(request, response);
	else
		runCgiScript(request, response);
}

// Controller를 Server 개수만큼만 만들자.

// POST /directory/youpla.bla HTTP/1.1
// Host: localhost
// Content-Length: 5

// aaaaa