#pragma once
#include "stdio.h"
#include <utility>

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
} // namespace queue_system