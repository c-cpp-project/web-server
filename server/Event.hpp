#include <sys/event.h>

#include <ostream>
#include <sstream>
#include <vector>
#define EVENTLIST_SIZE 1000

class Event {
 private:
  int kq;
  std::vector<struct kevent> changedList;
  struct kevent eventList[EVENTLIST_SIZE];

 public:
  Event();
  ~Event();
  struct kevent& operator[](int idx);

  void change(int socket, int16_t filter, uint16_t flags, uint32_t fflags,
              intptr_t data, void* udata);
  int create(void);
  void clearChangedEventList(void);
  bool initKqueue(void);
};

std::ostream& operator<<(std::ostream& out, struct kevent& val);
