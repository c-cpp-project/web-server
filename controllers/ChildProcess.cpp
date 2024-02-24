#include"ChildProcess.hpp"

std::unordered_map<int, clock_t>	ChildProcess::child;
double ChildProcess::max_cpu_time_used;

ChildProcess::ChildProcess(double max_cpu_time_used)
{
	child.clear();
	this->max_cpu_time_used = max_cpu_time_used;
}

void ChildProcess::insertChildProcess(pid_t pid)
{
	child.insert(std::make_pair(pid, clock()));
}

clock_t  ChildProcess::getChildProcessClock(pid_t pid)
{
	std::unordered_map<int, clock_t>::iterator iter;

	iter = child.find(pid);
	if (iter == child.end())
		return (0);
	return (iter->second);
}

void ChildProcess::earseChildProcess(pid_t pid)
{
	std::unordered_map<int, clock_t>::iterator iter;

	iter = child.find(pid);
	if (iter != child.end())
		child.erase(iter->first);
}

void ChildProcess::waitChildProcess(void)
{
	std::unordered_map<int, clock_t>::iterator iter;
	clock_t	endClock, startClock;
	double	cpu_time_used;
	pid_t	child_pid;
	int		status;

	endClock = clock();
	iter = child.begin();
	while (iter != child.end())
	{
		child_pid = iter->first;
		startClock = iter->second;
		iter++;
		if (child.count(child_pid) == 0)
			continue ;
		if (waitpid(child_pid, &status, WNOHANG) == 0) // 종료 되지 않았을 경우
		{
			cpu_time_used = ((double) (endClock - startClock)) / CLOCKS_PER_SEC;
			if (max_cpu_time_used < cpu_time_used)
			{
				std::cout << child_pid << ": EXCEED ALLOCATED CPU TIME " << cpu_time_used << "\n";
				kill(child_pid, SIGKILL);
			}
		}
		else // 종료 되었을 경우
		{
			child.erase(child_pid);
			if (WIFEXITED(status)) // 정상 종료
				std::cout << "Child process" << child_pid << " exited with status [" << WEXITSTATUS(status) << "]\n";
			else // 비정상 종료 -> fd가 닫히면 이벤트 감지에 의해 runBeanEvent가 실행된다.
				std::cout << "Child process" << child_pid << " exited with status [" << WEXITSTATUS(status) << "]\n";
		}
	}
}