#include"FrontController.hpp"

void    FrontController::run(HttpRequest tmp)
{
	std::vector<HttpRequest> 	*request;
	MultiRequest                multiRequest(tmp.getHeader("content-type"));
	HttpResponse    response(this->socketfd);

	request = multiRequest.makeRequest(tmp);
	for (int i = 0; i < static_cast<int>(request->size()); i++)
	{
		Controller      *controller;

		std::cout << "=============== [Request line] ===============\n";
		std::cout << "method: " << request->at(i).getMethod() << '\n';
		std::cout << "path: " << request->at(i).getPath() << '\n';
		std::cout << "query string: " << request->at(i).getQueryString() << '\n';
	
		std::cout << "=============== [Request Header] ==============\n";
		request->at(i).printAllHeader();

		std::cout << "=============== [Request Body] ===============\n";
		std::cout << request->at(i).getBody() << "\n";
		
		controller = request->at(i).getMethod() == "DELETE" ? (new DeleteController()) : (ControllerMapping::getController(request->at(i).getPath()));
		// std::cout << "getPath: [" << request->at(i).getPath() << "]" << "method: [" << request->at(i).getMethod() << "]\n"; 
		if (controller == nullptr || (request->at(i).getMethod() == "GET" && request->at(i).getQueryString() == ""))
		{
			if (HttpConfig::IsRedriectUri(request->at(i).getPath()) == true) // redirect
				response.redirect(request->at(i), response);
			else
				response.forward(request->at(i), response); // get
		}
		else // cgi: GET && POST, FILE POST, DELETE
		{
			controller->service(request->at(i), response);
			controller = nullptr;
		}
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