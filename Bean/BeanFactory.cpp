#include"BeanFactory.hpp"
std::map<std::string, Bean *> BeanFactory::beanRepository;

BeanFactory::BeanFactory()
{
    beanRepository.insert(std::pair<std::string, Bean *>("RECV", new RecvEventBean()));
    beanRepository.insert(std::pair<std::string, Bean *>("READ", new ReadEventBean()));
    beanRepository.insert(std::pair<std::string, Bean *>("WRITE", new WriteEventBean()));
    beanRepository.insert(std::pair<std::string, Bean *>("SEND", new SendEventBean()));
}

BeanFactory::~BeanFactory()
{
    std::map<std::string, Bean *>::iterator iter;

    for (iter = beanRepository.begin(); iter != beanRepository.end(); iter++)
        delete iter->second;
}

Bean    *BeanFactory::getBeanByName(std::string beanName)
{
    Bean    *bean;

    bean = beanRepository.at(beanName);
    return (bean);
}

void	BeanFactory::registerEvent(std::string beanName, HttpHandler *eventPending, Event *event)
{
    if (event == 0)
        return ;
    if (beanName == "READ" || beanName == "RECV") // 새로운 event 등록
        event->saveEvent(eventPending->getFd(), EVFILT_READ, 0, 0, 0, eventPending);
    else
        event->saveEvent(eventPending->getFd(), EVFILT_WRITE, 0, 0, 0, eventPending);
}

// event 발생 -> beenFactory.runBeanByName() 호출
// 가정: ServerConfiguration *_server_config: 하나의 요청부터 응답이 도달할 때까지 절대 사라지지  않는다. 
// 가정: Event *event: event는 객체는 하나다.
// 마지막으로 연산 과정에서 발생하는 Error에 관한 처리가 필요하다.
void    BeanFactory::runBeanByName(std::string beanName, HttpHandler *eventProcess, Event *event)
{
    Bean        *bean;
    // HttpHandler *eventPending;

    bean = getBeanByName(beanName);
    bean->runBeanEvent(eventProcess, event); // event 처리 -> event 등록
    delete eventProcess; // event 제거
}