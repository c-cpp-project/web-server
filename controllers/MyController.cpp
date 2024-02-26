#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::MyController(int masking) : Controller(masking)
{}

MyController::MyController(int masking, std::string mLocation) : Controller(masking, mLocation)
{}

MyController::~MyController()
{}

std::string	MyController::findDirectory(std::string fullpath, std::string page)
{
	DIR             *dir;
	struct dirent	*entry;
	size_t          idx;
	std::string     d_name;
	struct stat 	buf;

	if (fullpath[fullpath.length() - 1] == '/')
		fullpath = fullpath.substr(0, fullpath.length() - 1);
	int	i;
	for (i = fullpath.length() - 1; i >= 0; i--)
	{
		if (fullpath[i] == '/')
			break ;
	}
	std::string		directory = fullpath.substr(0, i);
	std::string		file = fullpath.substr(i + 1);
	std::cout << "MyController::findDirectory: ["<< directory << "], [" << file  << "]\n";
	dir = opendir(directory.c_str());
	if (dir == NULL)
	{
		std::cout << "Error opening directory\n";
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
	if (entry == NULL)
		throw "404";
	return (directory + "/" + d_name);
}

void	MyController::runCgiScript(HttpRequest &request, HttpResponse &response)
{
	ServerConfiguration *serverConfig = response.getServerConfiguration();
	// CGI 파일 경로 설정을 따로 해야 할 것으로 보임.. 아마도요?
	// request.setPath("CGI PATH");
	// std::cout << cgiPath << " = cgiPath\n";
	if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
		doGet(request, response);
	else if (request.getMethod() == "POST") // file upload
		doPost(request, response);
	else if (request.getMethod() == "DELETE")
	{
		long    idx;
		std::string removePath;

		for (idx = request.getPath().length() - 1; idx >= 0; idx--)
		{
			if (request.getPath()[idx] == '/')
				break ;
		}
		removePath = findDirectory(std::string(serverConfig->getUploadPath() + "/" + request.getPath().substr(idx + 1)), "");
		request.setPath(removePath);
		doDelete(request, response);
	}
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
		std::string index = location->getIndex()[0];
		std::string	root = location->getRoot();
		size_t		extension_idx = root.find("/");
		if (extension_idx != std::string::npos)
			root = root.substr(0, extension_idx);
		if (request.getPath() == serverConfig->findLocationUri(request.getPath())) // /root/index_file
		{
			staticPath = findDirectory(std::string(root + "/" + location->getIndex()[0]), index);
			std::cout << "staticPath: " << staticPath << "\n";
		}
		else // /root/file_name
		{
			std::string	mainChain;

			mainChain = serverConfig->findLocationUri(request.getPath());
			std::cout << request.getPath().substr(mainChain.length() + 1) << " = pendingElements\n";
			staticPath = findDirectory(std::string(root + "/" + request.getPath().substr(mainChain.length() + 1)), index);
		}
		std::cout << staticPath << " = staticPath\n";
		request.setPath(staticPath);
		response.putHeader("Content-Type", ResponseConfig::getContentType(staticPath));
		response.forward(request);
	}
}

void    MyController::service(HttpRequest &request, HttpResponse &response)
{
	std::string cgiPath;
	ServerConfiguration *serverConfig = response.getServerConfiguration();
	Location    *location;

	std::cout << "MyController::service" << "\n";
	if (isAcceptableMethod(request.getMethod()) == false)
		throw "405";
	// ============================ 애매한 부분 ============================ //
	// 1. redirectPath는 어떻게 설정하는가?
	// 2. CGI 파열 경로는 어떻게 설정하는가? -> 42번 줄 확인 바람!
	// 3. getResourcePath는 어떻게 설정되고 무엇을 반환하는가?
	// ================================================================== //
	if ((request.getMethod() == "GET") && request.getQueryString() == "")
		runService(request, response);
	else
		runCgiScript(request, response);
}

// Controller를 Server 개수만큼만 만들자.