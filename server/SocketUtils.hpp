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
};

#endif
