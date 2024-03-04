#include "FrontController.hpp"

FrontController::FrontController(int socketfd,
                                 ServerConfiguration *serverConfig,
                                 Event *event) {
  this->socketfd = socketfd;
  this->serverConfig = serverConfig;
  this->event = event;
}

FrontController::FrontController(int socketfd, int fd) {
  this->socketfd = socketfd;
  int tmp;
  tmp = fd;
}

void FrontController::run(HttpRequest tmp) {
  std::vector<HttpRequest> *request;
  MultiRequest multiRequest(tmp.getHeader("content-type"));
  HttpResponse *response;

  request = multiRequest.makeRequest(tmp);
  for (int i = 0; i < static_cast<int>(request->size()); i++) {
    Controller *controller;
    std::string location;

    // std::cout << "=============== [Request line] ===============\n";
    // std::cout << "method: " << request->at(i).getMethod() << '\n';
    // std::cout << "path: " << request->at(i).getPath() << '\n';
    // std::cout << "query string: " << request->at(i).getQueryString() << '\n';

    // std::cout << "=============== [Request Header] ==============\n";
    request->at(i).printAllHeader();

    // std::cout << "=============== [Request Body] ===============\n";
    // // std::cout << request->at(i).getBody() << "\n";
    // std::cout << "=============== [Request Body Length] ===============\n";
    // std::cout << request->at(i).getBody().length() << "\n";
    // std::cout << "===============================================\n";
    // std::cout << this->socketfd << ": this->socketfd\n";
    response = new HttpResponse(this->socketfd, serverConfig, event);
    controller = ControllerMapping::getController(serverConfig->getPort(),
                                                  request->at(i).getPath());
    controller->service(request->at(i),
                        (*response));  // CGI에서 대한 I/O 작업: READ, WRITE
    controller = nullptr;
  }
  // std::cout << "====================================================\n\n";
  // response.flush(); // response I/O HTTP 작업: WRITE
  // std::cout << "====================================================\n\n";
  request->clear();
  delete request;
}

FrontController::FrontController(int socketfd) { this->socketfd = socketfd; }

FrontController::FrontController() {}

FrontController::~FrontController() {}