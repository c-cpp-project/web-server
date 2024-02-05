#include "Event.hpp"

#include <fcntl.h>

#include <iostream>

Event::Event() : kq(0) {}

Event::~Event() {}

struct kevent& Event::operator[](int idx) {
  if (0 <= idx && idx < EVENTLIST_SIZE)
    return eventList[idx];
  else {
    std::cout << "eventList index Error" << std::endl;
    exit(1);
  }
}

void Event::change(int fd, int16_t filter, uint16_t flags, uint32_t fflags,
                   intptr_t data, void* udata) {
  struct kevent event;
  EV_SET(&event, fd, filter, flags, fflags, data, udata);
  changedList.push_back(event);
}

bool Event::initKqueue(void) {
  if (kq != 0) {
    std::cout << "kqueue already initialized" << std::endl;
    return true;
  }
  this->kq = kqueue();
  if (this->kq == -1) return true;
  return false;
}

int Event::create(void) {
  int res;

  res = kevent(kq, &changedList[0], changedList.size(), eventList,
               EVENTLIST_SIZE, NULL);
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
