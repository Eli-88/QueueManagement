#pragma once
#include "stdio.h"
#include <utility>
#include <thread>
#include <future>
#include <functional>
#include <optional>

namespace queue_system {

template <class Exception>
void check_and_throw(bool condition, const char *fmt) {
  if (not condition) {
    throw Exception(fmt);
  }
}

template <class Exception, typename... Args>
void check_and_throw(bool condition, const char *fmt, Args &&... args) {
  if (not condition) {
    char buf[256]{};
    snprintf(buf, 255, fmt, std::forward<Args>(args)...);
    throw Exception(fmt);
  }
}

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class DetachableThread {
public:
  DetachableThread() = default;
  DetachableThread(const char *name, const std::function<void()> &impl);
  DetachableThread(const DetachableThread &) = delete;
  DetachableThread(DetachableThread &&others)
      : thread_(std::move(others.thread_)) {}
  DetachableThread &operator=(const DetachableThread &) = delete;
  DetachableThread &operator=(DetachableThread &&) = default;

private:
  std::thread thread_;
};

namespace server {
struct ServerException : public std::runtime_error {
  ServerException(const char *msg) : std::runtime_error(msg) {}
};

struct PollException : public std::runtime_error {
  PollException(const char *msg) : std::runtime_error(msg) {}
};
} // namespace server

} // namespace queue_system