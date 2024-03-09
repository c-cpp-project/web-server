#include "SocketUtils.hpp"

void SocketUtils::exitWithPerror(const std::string& msg) {
  perror(msg.c_str());
  exit(EXIT_FAILURE);
}