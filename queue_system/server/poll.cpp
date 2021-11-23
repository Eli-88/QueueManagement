#include "common.h"
#include "poll.h"
#include "session.h"
#include <iostream>
#include <utility>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace queue_system {
namespace server {

template <typename... Args>
static void check_epoll_err(int err, const char *fmt, Args &&... args) {
  check_and_throw<Poll::PollException>((err != -1), fmt,
                                       std::forward<Args>(args)...);
}

Poll::Poll() : epollFd_(epoll_create1(0)) {
  if (-1 == epollFd_) {
    throw PollException("epoll create failed");
  }
}

Poll::~Poll() { close(epollFd_); }

void Poll::add(int fd, SessionPtr request) {
  allSessions_[fd].reset();
  allSessions_[fd] = request;
  struct epoll_event ev {};
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  const auto err = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
  check_epoll_err(err, "epoll fail to add fd[%d]", fd);
}

void Poll::remove(int fd) {
  allSessions_[fd].reset();
  struct epoll_event ev {};
  ev.data.fd = fd;
  const auto err = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &ev);
  check_epoll_err(err, "epoll fail to remove fd[%d]", fd);
}

std::vector<Poll::SessionPtr> Poll::poll_once() {
  struct epoll_event ev[POLL_MAX_EVENTS];
  const auto num_ev =
      epoll_wait(epollFd_, ev, POLL_MAX_EVENTS, POLL_TIMEOUT_SEC);
  check_epoll_err(num_ev, "epoll poll once failed");
  std::vector<SessionPtr> requests;
  for (int i = 0; i < num_ev; i++) {
    if (EPOLLIN == ev[i].events) {
      const auto fd = ev[i].data.fd;
      auto req = allSessions_.find(fd);
      check_and_throw<PollException>(
          (req != allSessions_.end()),
          "this should not happen: unable to find fd[%d] in allRequest", fd);
      requests.push_back(req->second);
    }
  }
  return requests;
}

} // namespace server

} // namespace queue_system
