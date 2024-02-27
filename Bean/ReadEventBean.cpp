#include "ReadEventBean.hpp"

ReadEventBean::ReadEventBean() {}
ReadEventBean::~ReadEventBean() {}

int ReadEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  HttpResponse *response;
  std::string body;
  int readFd;

  readFd = httpHandler->getFd();
  response = &httpHandler->getHttpResponse();
  if (response == 0)  // flush -> delete.
    return 0;
  try
  {
    body = response->readFile(readFd);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    errorSaveEvent(httpHandler, event);
  }
  responseSaveEvent(body, httpHandler, event);
  delete httpHandler;
  return 0;
}

void ReadEventBean::errorSaveEvent(HttpHandler *httpHandler, Event *event) {
  HttpResponse &response = httpHandler->getHttpResponse();
  HttpRequest empty;

  std::cout << "ReadEventBean::errorSaveEvent\n";
  response.setStatusCode("500");
  response.forward(empty);
}

void ReadEventBean::responseSaveEvent(std::string body,
                                      HttpHandler *httpHandler, Event *event) {
  std::stringstream ss;
  std::string bodyLength;
  ServerConfiguration *serverConfig;
  HttpResponse &response = httpHandler->getHttpResponse();
  HttpRequest &request = httpHandler->getHttpRequest();

  serverConfig = response.getServerConfiguration();
  response.putHeader("Server", serverConfig->getServerName());
  response.putHeader("Date", ResponseConfig::getCurrentDate());
  if (request.getParameter("Range") != "" && request.getMethod() == "GET")
    body = response.readRangeQuery(request.getParameter("Range"), body);
  ss << body.length();
  bodyLength = ss.str();
  response.putHeader("Content-Length", bodyLength);
  // response.putHeader("Content-Type", "image/png");
  response.sendBody(body);  // this->buffer에 string으로 모두 담긴다.
  std::cout << "ReadEventBean::response -> saveEvent\n";
  std::cout << bodyLength << " = \"Content-Length\"\n";

  event->saveEvent(response.getSockfd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   new HttpHandler(response.getSockfd(),
                                   response));  // EVFILT_READ, EVFILT_WRITE
}