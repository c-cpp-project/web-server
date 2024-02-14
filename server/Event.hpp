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

  void saveEvent(int socket, int16_t filter, uint16_t flags, uint32_t fflags,
                 intptr_t data, void* udata);
  int create(void);
  void clearChangedEventList(void);
  bool initKqueue(void);
  void registerServerEvent(int serverSocket, void* serverConfiguration);
  void registerEnabledReadEvent(int clientSocket, void* handler);
  void registerDisabledWriteEvent(int clientSocket, void* handler);
  void enableWriteEvent(int clientSocket, void* handler);
  void disableWriteEvent(int clientSocket, void* handler);
  void enableReadEvent(int clientSocket, void* handler);
};

std::ostream& operator<<(std::ostream& out, struct kevent& val);
