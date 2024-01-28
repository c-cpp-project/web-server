#include "SocketUtils.hpp"

void SocketUtils::exitWithPerror(const std::string& msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

void SocketUtils::changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
                               uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
    struct kevent event;
    EV_SET(&event, ident, filter, flags, fflags, data, udata);
    changeList.push_back(event);
}

void SocketUtils::disconnectClient(int clientFd, std::map<int, std::string>& clients) {
    std::cout << "Client disconnected: " << clientFd << std::endl;
    close(clientFd);
    clients.erase(clientFd);
}
