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
  std::cout << "ReadEventBean::runBeanEvent = [" << readFd << ",";
  buf_size = serverConfig->getClientRequestSize("");
  temp_buffer = new char[buf_size];
  readByte = read(readFd, temp_buffer, buf_size);
  if (readByte < 0)
  {
    errorSaveEvent(httpHandler, event);
    buffers[readFd] = "";
    delete[] temp_buffer;
  }
  buffers[readFd] += std::string(temp_buffer, readByte);
  std::cout << readByte << " , " << buffers[readFd].length() << "]\n";
  delete[] temp_buffer;
  if (readByte > 0 && buffers[readFd].length() < httpHandler->getBodySize())
    return (readByte);
  else
  {
    responseSaveEvent(buffers[readFd], httpHandler, event);
    buffers[readFd] = "";
  }
  event->saveEvent(httpHandler->getFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
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
  std::cout << "ReadEventBean::responseSaveEvent\n";
  ServerConfiguration *serverConfig;
  HttpResponse &response = httpHandler->getHttpResponse();
  HttpRequest &request = httpHandler->getHttpRequest();

  if (request.getParameter("Range") != "" && request.getMethod() == "GET")
    body = response.readRangeQuery(request.getParameter("Range"), body);
  response.sendBody(body);
  event->saveEvent(response.getSockfd(), EVFILT_WRITE, EV_ENABLE, 0, 0,
                   new HttpHandler(response.getSockfd(), response.getByteDump(),
                                   serverConfig));  // EVFILT_READ, EVFILT_WRITE
}