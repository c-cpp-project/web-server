#include"FrontController.hpp"
#include"../server/ServerConfiguration.hpp"

void    FrontController::run(HttpRequest tmp, ServerConfiguration serverConfig)
{
	std::vector<HttpRequest> 	*request;
	MultiRequest                multiRequest(tmp.getHeader("content-type"));
	HttpResponse    response(this->socketfd);

	request = multiRequest.makeRequest(tmp);
	for (int i = 0; i < static_cast<int>(request->size()); i++)
	{
		Controller      *controller;
		std::string		location;

		std::cout << "=============== [Request line] ===============\n";
		std::cout << "method: " << request->at(i).getMethod() << '\n';
		std::cout << "path: " << request->at(i).getPath() << '\n';
		std::cout << "query string: " << request->at(i).getQueryString() << '\n';
	
		std::cout << "=============== [Request Header] ==============\n";
		request->at(i).printAllHeader();

		std::cout << "=============== [Request Body] ===============\n";
		std::cout << request->at(i).getBody() << "\n";
		
		// ServerConfiguration	serverConfig;
		std::string			path;
		Controller			*controller;
		
		controller = ControllerMapping::getController(serverConfig.getPort(), request->at(i).getPath());
		// path = serverConfig.getResourcePath(request->at(i).getPath());
		// if (path != "")
		// 	request->at(i).setPath(path);
		controller->service(request->at(i), response, serverConfig);
		controller = nullptr;
	}
	std::cout << "====================================================\n\n";
	response.flush(); // response header 수정할게요
	std::cout << "====================================================\n\n";
	delete request;
}

FrontController::FrontController(int kqueuefd, int socketfd)
{
	this->kqueuefd = kqueuefd;
	this->socketfd = socketfd;
}

FrontController::FrontController(int socketfd)
{
	this->socketfd = socketfd;
}

FrontController::FrontController()
{}

FrontController::~FrontController()
{}