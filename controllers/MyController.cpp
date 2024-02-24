#include"MyController.hpp"

MyController::MyController() : Controller()
{}

MyController::MyController(int masking) : Controller(masking)
{}

MyController::MyController(int masking, std::string mLocation) : Controller(masking, mLocation)
{}

MyController::~MyController()
{}

std::string	MyController::findDirectory(std::string directory, std::string file)
{
    DIR             *dir;
    struct dirent	*entry;
    size_t          idx;
    std::string     d_name;
    char            fullpath[1024];

    std::cout << "MyController::findDirectory: "<< directory << ", " << file  << "\n";
    if (access(std::string(directory +  "/" + file).c_str(), F_OK) == 0)
        return (directory + "/" + file);
    // getcwd(fullpath, 1023);
    // directory = std::string(fullpath) + "/" + directory.substr(0, directory.length() - 1);
    dir = opendir(directory.c_str());
    if (dir == NULL || file.find(".") != std::string::npos)
    {
        std::cout << "Error opening directory\n";
        throw "404";
    }
    // file에 확장자 없을 경우: 가장 처음으로 만나는 동일한 이름의 파일에 대응된다.
    while ((entry = readdir(dir)) != NULL)
    {
        d_name = std::string(entry->d_name);
        d_name = d_name.substr(0, d_name.find("."));
        std::cout << d_name << " = repository file\n";
        if (d_name == file)
            break ;
    }
    if (entry == NULL)
        throw "404";
    return (directory + "/" + std::string(entry->d_name));
}

void    MyController::service(HttpRequest &request, HttpResponse &response)
{
    std::string redirectPath;
    std::string staticPath;
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
    if (request.getMethod() == "HEAD" || request.getMethod() == "GET" && request.getQueryString() == "")
    {
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
            if (request.getPath() == serverConfig->findLocationUri(request.getPath())) // /root/index_file
                staticPath = serverConfig->getResourcePath(request.getPath());
            else // /root/file_name
            {
                long    idx;

                for (idx = request.getPath().length() - 1; idx >= 0; idx--)
                {
                    if (request.getPath()[idx] == '/')
                        break ;
                }
                staticPath = findDirectory(location->getRoot().substr(0, location->getRoot().length() - 1), request.getPath().substr(idx + 1));
            }
            std::cout << staticPath << " = staticPath\n";
            request.setPath(staticPath);
            response.putHeader("Content-Type", ResponseConfig::getContentType(staticPath));
            response.forward(request);
        }
    }
    else
    {
        // CGI 파일 경로 설정을 따로 해야 할 것으로 보임.. 아마도요?
        // request.setPath("CGI PATH");
        // std::cout << cgiPath << " = cgiPath\n";
        if (request.getMethod() == "GET" || request.getHeader("CONTENT-TYPE") == "application/x-www-form-urlencoded") // get, post
            doGet(request, response);
        else if (request.getMethod() == "POST") // file upload
            doPost(request, response);
        else if (request.getMethod() == "DELETE")
        {
            this->mLocation = serverConfig->getUploadPath();
            request.setPath(this->mLocation);
            std::cout << request.getPath() << "\n";
            doDelete(request, response);
        }
    }
}

// Controller를 Server 개수만큼만 만들자.