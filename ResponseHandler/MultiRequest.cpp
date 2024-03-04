#include "MultiRequest.hpp"

MultiRequest::MultiRequest(std::string contentType) {
  std::string multipart;

  multipart = "multipart/form-data;";
  if (contentType.find(multipart) == std::string::npos)
    this->isMultipart = false;
  else {
    this->isMultipart = true;
    this->boundary =
        "--" + contentType.substr(contentType.find("boundary=") +
                                  std::string("boundary=").length());
    // std::cout << "[" << this->boundary << "]\n";
  }
}
// cgi에서 contentDisposition를 분해해서 filename을 뽑자.
std::vector<HttpRequest> *MultiRequest::makeRequest(HttpRequest request) {
  size_t cur;
  size_t next;
  std::vector<HttpRequest> *requestVec;
  std::string body;

  requestVec = new std::vector<HttpRequest>();
  if (this->isMultipart == false)
    requestVec->push_back(request);
  else {
    body = request.getBody();
    cur = this->boundary.length() + std::string("\r\n").length();
    next = body.find(this->boundary, cur);
    while (next != std::string::npos) {
      HttpRequest child;

      child = request;
      fillEachRequest(child, body.substr(cur, next - cur));  // [cur, next)
      requestVec->push_back(child);
      cur = next + this->boundary.length() + std::string("\r\n").length();
      next = body.find(this->boundary, cur);
    }
  }
  return requestVec;
}

void MultiRequest::fillEachRequest(HttpRequest &request, std::string data) {
  size_t cur;
  size_t next;
  std::stringstream ss;
  std::string body;

  // std::cout << "fillEachRequest [" << data << "]\n";

  request.removeHeader("Content-Type");
  cur = 0;
  while (true) {
    next = data.find("\r\n", cur);
    if (next == cur) break;
    std::string line = data.substr(cur, next - cur);
    if (request.addHeader(line) == FAILURE) throw "400";
    cur = next + std::string("\r\n").length();
  }
  cur += std::string("\r\n").length();
  body = data.substr(cur);
  ss << body.length();
  request.setHeader("Content-Length", ss.str());
  request.setRequestBody(body);
}

MultiRequest::MultiRequest() {}

MultiRequest::~MultiRequest() {}
