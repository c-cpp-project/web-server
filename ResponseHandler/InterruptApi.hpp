// API의 결과를 response 객체에 담아서 내보낸다.
// pipefd의 값을 읽어서 완료되면 socketpd로 내보낸다.

#include"HttpResponse.hpp"
#include"../ResponseConfig.hpp"
#ifndef	INTERRUPT_API_HPP
# define INTERRUPT_API_HPP

class HttpResponse;
class InterruptApi
{
private:
	int				pipefd;
	int 			sockfd;
	unsigned long	max_size;
public:
	void    callResponse();
	InterruptApi(int pipefd, int sockfd);
	InterruptApi();
	~InterruptApi();
};
#endif