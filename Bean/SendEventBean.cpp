#include "SendEventBean.hpp"
#include<sys/socket.h>

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

#define BUF_SIZE 1024 * 1024
int SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
	ServerConfiguration 	*serverConfig;
	std::string				dump;
	int						socketfd;
	int						ret;
	int						buf_size;
	char					buffer[BUF_SIZE];

	serverConfig = httpHandler->getServerConfiguration();
	socketfd = httpHandler->getFd();
	dump = httpHandler->getData();
	std::cout << serverConfig << " = SendEventBean::runBeanEvent\n";
	buf_size = BUF_SIZE;
	if (dump.length() < BUF_SIZE)
		buf_size = dump.length();
	std::cout << socketfd << ", " << dump.length() << ": size => " << buf_size << "\n";
	memcpy(buffer, dump.substr(0, buf_size).c_str(), buf_size);
	std::cout << buf_size << "\n";
	ret = write(socketfd, buffer, buf_size);
	if (ret == 0)
	{
		event->saveEvent(socketfd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
		event->saveEvent(socketfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
										new HttpHandler(socketfd, serverConfig));
		delete httpHandler;
		std::cout << "================== SEND DONE ============================\n";
	}
	else if (ret > 0)
		httpHandler->setData(dump.substr(ret));
	std::cout << ret << "\n";
	return (ret);
}