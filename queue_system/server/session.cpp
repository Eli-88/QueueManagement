#include "session.h"
#include "poll.h"
#include <sys/socket.h>
#include <unistd.h>

namespace queue_system {
namespace server {
Session::Session(int fd, Callback handler, ErrCallback errHandler)
    : fd_(fd), handler_(handler), errHandler_(errHandler) {}
Session::~Session() {
  shutdown(fd_, SHUT_RDWR);
  close(fd_);
}
int Session::fd() const { return fd_; }
} // namespace server
} // namespace queue_system