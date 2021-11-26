#include "poll.h"
#include "common.h"
#include "session.h"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <utility>

#ifdef __linux__
#include <sys/epoll.h>
#endif
#ifdef __APPLE__
#include <sys/event.h>
#endif

namespace queue_system {
namespace server {

namespace {
template <typename... Args>
static void check_epoll_err(int err, const char *fmt, Args &&...args) {
  check_and_throw<PollException>((err != -1), fmt,
                                       std::forward<Args>(args)...);
}
} // namespace

Poll::Poll() {
#ifdef __linux__
  pollFd_ = epoll_create1(0);
#endif
#ifdef __APPLE__
  pollFd_ = kqueue();
#endif
  if (-1 == pollFd_) {
    throw PollException("epoll create failed");
  }
}

Poll::~Poll() { close(pollFd_); }

void Poll::add(int fd, SessionPtr request) {
  allSessions_[fd].reset();
  allSessions_[fd] = request;

#ifdef __linux__
  struct epoll_event ev {};
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  const auto err = epoll_ctl(pollFd_, EPOLL_CTL_ADD, fd, &ev);
#endif
#ifdef __APPLE__
  struct kevent ev {};
  EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
  const auto err = kevent(pollFd_, &ev, 1, nullptr, 0, nullptr);
#endif
  check_epoll_err(err, "epoll fail to add fd[%d]", fd);
}

void Poll::remove(int fd) {
  allSessions_[fd].reset();
#ifdef __linux__
  struct epoll_event ev {};
  ev.data.fd = fd;
  const auto err = epoll_ctl(pollFd_, EPOLL_CTL_DEL, fd, &ev);
#endif
#ifdef __APPLE__
  struct kevent ev {};
  EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  const auto err = kevent(pollFd_, &ev, 1, nullptr, 0, nullptr);
#endif
  check_epoll_err(err, "epoll fail to remove fd[%d]", fd);
}

std::vector<Poll::SessionPtr> Poll::poll_once() {
  std::vector<SessionPtr> requests;
#ifdef __linux__
  struct epoll_event ev[POLL_MAX_EVENTS];
  const auto num_ev =
      epoll_wait(pollFd_, ev, POLL_MAX_EVENTS, POLL_TIMEOUT_SEC);
  check_epoll_err(num_ev, "epoll poll once failed");
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
#endif
#ifdef __APPLE__
  struct kevent ev[POLL_MAX_EVENTS];
  timespec timeout{0, POLL_TIMEOUT_SEC};
  const auto num_ev =
      kevent(pollFd_, nullptr, 0, ev, POLL_MAX_EVENTS, &timeout);
  check_epoll_err(num_ev, "kqueue polling failed");
  for (int i = 0; i < num_ev; i++) {
    if (ev[i].filter == EVFILT_READ) {
      const int fd = ev[i].ident;
      auto req = allSessions_.find(fd);
      check_and_throw<PollException>(
          (req != allSessions_.end()),
          "this should not happen: unable to find fd[%d] in allRequest", fd);
      requests.push_back(req->second);
    }
  }
#endif
  return requests;
}

} // namespace server

} // namespace queue_system
