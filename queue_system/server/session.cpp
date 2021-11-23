#include "session.h"
#include "poll.h"
#include <unistd.h>

namespace queue_system {
namespace server {
Session::Session(int fd, Callback handler) : fd_(fd), handler_(handler) {}
Session::~Session() { close(fd_); }
int Session::fd() const { return fd_; }
} // namespace server
} // namespace queue_system