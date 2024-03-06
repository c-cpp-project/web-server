#include "ReadEventBean.hpp"

ReadEventBean::ReadEventBean() {}
ReadEventBean::~ReadEventBean() {}

std::map<int, std::string> ReadEventBean::buffers;

int ReadEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
  int readFd;
  int readByte;
  char *temp_buffer;
  HttpResponse &response = httpHandler->getHttpResponse();
  ServerConfiguration *serverConfig = response.getServerConfiguration();
  int buf_size;

  readFd = httpHandler->getFd();
  std::cout << "ReadEventBean::runBeanEvent = [" << readFd << ", ";
  buf_size = serverConfig->getClientRequestSize("");
  temp_buffer = new char[buf_size];
  readByte = read(readFd, temp_buffer, buf_size);
  if (readByte < 0 || (readByte == 0 && buffers[readFd] == ""))
  {
    errorSaveEvent(httpHandler, event);
    buffers[readFd] = "";
    delete[] temp_buffer;
  }
  else
  {
    buffers[readFd].append(temp_buffer, readByte);
    std::cout << readByte << " , " << buffers[readFd].length() << " == " << httpHandler->getBodySize() << "]\n";
    delete[] temp_buffer;
    if ((httpHandler->getBodySize() == 0 && readByte != 0) || (httpHandler->getBodySize() != 0 && buffers[readFd].length() != httpHandler->getBodySize()))
      return (readByte);
    responseSaveEvent(buffers[readFd], httpHandler, event);
    buffers[readFd] = "";
  }
  close(readFd);
  event->saveEvent(httpHandler->getFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
  delete httpHandler;
  httpHandler = 0;
  std::cout << "ReadEventBean::runBeanEvent DONE\n";
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
  std::cout << "ReadEventBean::responseSaveEvent\n";
  HttpResponse &response = httpHandler->getHttpResponse();
  HttpRequest &request = httpHandler->getHttpRequest();
  ServerConfiguration *serverConfig = response.getServerConfiguration();

  if (request.getParameter("Range") != "" && request.getMethod() == "GET" && request.getQueryString() == "")
    body = response.readRangeQuery(request.getParameter("Range"), body);
  response.sendBody(body, (request.getQueryString() == "" && request.getMethod() == "GET" ||\
  ("400" <= response.getStatusCode() && response.getStatusCode() <= "500")));
  event->saveEvent(response.getSockfd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   new HttpHandler(response.getSockfd(), response.getByteDump(),
                                   serverConfig));  // EVFILT_READ, EVFILT_WRITE
}