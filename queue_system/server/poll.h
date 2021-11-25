#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace queue_system {

namespace server {
// forward declaration
class Session;

class Poll {
public:
  using SessionPtr = std::shared_ptr<Session>;

  struct PollException : public std::runtime_error {
    PollException(const char *msg) : std::runtime_error(msg) {}
  };

public:
  Poll();
  ~Poll();
  void add(int fd, SessionPtr session);
  void remove(int fd);
  std::vector<SessionPtr> poll_once();

private:
  int pollFd_;
  std::unordered_map<int, SessionPtr> allSessions_;
  static constexpr int POLL_MAX_EVENTS = 30;
  static constexpr int POLL_TIMEOUT_SEC = 1;
};
} // namespace server

} // namespace queue_system