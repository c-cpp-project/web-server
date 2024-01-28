#ifndef SOCKETUTILS_HPP
#define SOCKETUTILS_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>

class SocketUtils {
public:
    static void exitWithPerror(const std::string& msg);
    static void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter,
                             uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    static void disconnectClient(int clientFd, std::map<int, std::string>& clients);
};

#endif
