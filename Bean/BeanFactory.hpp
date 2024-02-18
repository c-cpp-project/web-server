
#ifndef BEAN_FACTORY_HPP
# define BEAN_FACTORY_HPP
#include"Bean.hpp"
#include"ReadEventBean.hpp"
#include"SendEventBean.hpp"
#include"WriteEventBean.hpp"
#include"RecvEventBean.hpp"
#include<map>
#include<iostream>
#include<vector>
#include"HttpHandler.hpp"

class HttpHandler;
class Bean;
class BeanFactory
{
private:
	static std::map<std::string, Bean *> beanRepository;
	static Bean    *getBeanByName(std::string beanName);
public:
	BeanFactory(); // Bean을 생성한다.
	~BeanFactory(); // Bean을 소멸시킨다.

	static	void    runBeanByName(std::string beenName, HttpHandler *httpHandler, Event *event);
};
#endif
// Bean factory

// 1. Bean은 재사용 가능한 객체이다.
// 2. Event는 서로 독립적이고 동일한 분류의 Event는 동일한 메서드를 호출한다. 따라서 Event는 Bean이다. (단지 그뿐.)
// 3. Request, respons만 달라진다.

// 재사용 가능한 객체, Bean을 호출을 통해 사용하자.