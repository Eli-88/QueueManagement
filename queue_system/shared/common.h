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

class Thread {
public:
  Thread(const std::function<void()> &impl);
  Thread(const Thread &) = delete;
  Thread(Thread &&others) : thread_(std::move(others.thread_)) {}
  void join();

private:
  std::thread thread_;
  std::future<void> future_;
};

} // namespace queue_system