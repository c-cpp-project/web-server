
#ifndef CHILD_PROCESS__HPP
# define CHILD_PROCESS__HPP

#include <unordered_map>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <sys/time.h>

class   ChildProcess
{
private:
    static std::unordered_map<int, timeval>	child;
    static long max_cpu_time_used;
public:
    ChildProcess(long max_cpu_time_used);
    static void insertChildProcess(pid_t pid);
    static void earseChildProcess(pid_t pid);
    static void waitChildProcess(void);
};
#endif