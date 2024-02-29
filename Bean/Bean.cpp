#include"Bean.hpp"

Bean::Bean() {}

Bean::~Bean() {}

bool Bean::isEndOfFile(int fd)
{
  off_t currentPosition = lseek(fd, 0, SEEK_CUR);
  off_t endOfFile = lseek(fd, 0, SEEK_END);

  std::cout << currentPosition << ", " << endOfFile << "\n";
  return (currentPosition == endOfFile);
}