#pragma once

#include <memory>
#include <vector>

namespace queue_system {

namespace server {
// forward declaration
class Session;

class Poll {
public:
  Poll();
  ~Poll();
  void add(int fd);
  void remove(int fd);
  std::vector<int> poll_once();

private:
  int pollFd_;
  static constexpr int POLL_MAX_EVENTS = 30;
  static constexpr int POLL_TIMEOUT_SEC = 1;
};
} // namespace server

} // namespace queue_system