
#ifndef CHILD_PROCESS__HPP
# define CHILD_PROCESS__HPP

#include <ctime>
#include <unordered_map>
#include <sys/types.h>
#include <signal.h>
#include <iostream>

class   ChildProcess
{
private:
    static std::unordered_map<int, clock_t>	child;
    static double max_cpu_time_used;
public:
    ChildProcess(double max_cpu_time_used);
    static void insertChildProcess(pid_t pid);
    static clock_t  getChildProcessClock(pid_t pid);
    static void earseChildProcess(pid_t pid);
    static void waitChildProcess(void);
};
#endif