#pragma once
#include <memory>
#include <functional>

namespace queue_system {
namespace server {

// forward declaration
class Poll;

class Session {
public:
  using Callback = std::function<void(const std::string&)>;
  Session(int fd, Callback handler);
  ~Session();

  virtual bool handle_request() = 0;
  int fd() const;

protected:
  int fd_;
  Callback handler_;
};
} // namespace server
} // namespace queue_system