#include "Event.hpp"

#include <fcntl.h>

#include <iostream>

Event::Event() : kq(0) {}

Event::~Event() {}

struct kevent& Event::operator[](int idx) {
  if (0 <= idx && idx < EVENTLIST_SIZE)
    return eventList[idx];
  else {
    std::cout << "[Error] eventList index Error" << std::endl;
    exit(1);
  }
}

void Event::saveEvent(int fd, int16_t filter, uint16_t flags, uint32_t fflags,
                      intptr_t data, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, filter, flags, fflags, data, udata);
  changedList.push_back(event);
}

bool Event::initKqueue(void) {
  if (kq != 0) {
    std::cout << "[WARN] kqueue already initialized" << std::endl;
    return true;
  }
  this->kq = kqueue();
  if (this->kq == -1) return true;
  return false;
}

int Event::create(void) {
  int res;
  struct timespec timeout;
  timeout.tv_sec = MAX_CPU_TIME_USED;  // 5초
  timeout.tv_nsec = 0;

  res = kevent(kq, &changedList[0], changedList.size(), eventList,
               EVENTLIST_SIZE, &timeout);  // TODO: timeout 설정 해야할까?
  return res;
}

void Event::clearChangedEventList(void) { changedList.clear(); }

std::ostream& operator<<(std::ostream& out, struct kevent& val) {
  std::stringstream stream;
  stream << std::hex << val.flags;
  std::string _filter;

  switch (val.filter) {
    case (EVFILT_READ):
      _filter = "READ";
      break;
    case (EVFILT_WRITE):
      _filter = "WRITE";
      break;
    default:
      _filter = "ERROR";
      break;
  }
  out << "ident : " << val.ident << " filter : " << _filter << " flags : 0x"
      << stream.str() << " data: " << val.data << std::endl;
  return out;
}

void Event::registerServerEvent(int serverSocket, void* serverConfiguration) {
  saveEvent(serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
            serverConfiguration);
}

void Event::registerEnabledReadEvent(int clientSocket, void* handler) {
  saveEvent(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, handler);
}

void Event::registerDisabledWriteEvent(int clientSocket, void* handler) {
  saveEvent(clientSocket, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, handler);
};

void Event::enableWriteEvent(int clientSocket, void* handler) {
  saveEvent(clientSocket, EVFILT_WRITE, EV_ENABLE, 0, 0, handler);
}

void Event::disableWriteEvent(int clientSocket, void* handler) {
  saveEvent(clientSocket, EVFILT_WRITE, EV_DISABLE, 0, 0, handler);
};

void Event::enableReadEvent(int clientSocket, void* handler) {
  saveEvent(clientSocket, EVFILT_READ, EV_ENABLE, 0, 0, handler);
}