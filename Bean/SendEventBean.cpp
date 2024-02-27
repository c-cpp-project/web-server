#include "SendEventBean.hpp"

SendEventBean::SendEventBean() {}
SendEventBean::~SendEventBean() {}

int SendEventBean::runBeanEvent(HttpHandler *httpHandler, Event *event) {
	ServerConfiguration 	*serverConfig;
	std::string				dump;
	int						socketfd;
	int						ret;

	serverConfig = httpHandler->getServerConfiguration();
	socketfd = httpHandler->getFd();
	dump = httpHandler->getData();

	std::cout << serverConfig << " = SendEventBean::runBeanEvent\n";
	ret = send(socketfd, dump.c_str(), dump.length(), 0);
	if (ret <= 0)
	{
		event->saveEvent(socketfd, EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
		event->saveEvent(socketfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
										new HttpHandler(socketfd, serverConfig));
		delete httpHandler;
		std::cout << "================== SEND DONE ============================\n";
		return (0);
	}
	else
		httpHandler->setData(dump.substr(ret));
	return (ret);
}