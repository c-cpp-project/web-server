#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP

#include"../RequestHandler/HttpRequest.hpp"
#include"../RequestHandler/HttpResponse.hpp"
#include<string>

class   HttpRequest;
class   HttpResponse;
class Controller
{
private:
public:
	void    service(HttpRequest &request, HttpResponse &response);
	virtual void    doGet(HttpRequest &request, HttpResponse &response);
	virtual void	doPost(HttpRequest &request, HttpResponse &response);
	virtual	void	doDelete(HttpRequest &request, HttpResponse &response);
	virtual ~Controller();
	Controller();
};

#endif
// 자식들이 부모의 service를 통해 자신들의 메서드를 사용한다.

