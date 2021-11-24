#pragma once
#include <functional>
#include <memory>

namespace queue_system {
namespace server {

// forward declaration
class Poll;

class Session : public std::enable_shared_from_this<Session> {
public:
  using Callback =
      std::function<void(const std::string &, std::shared_ptr<Session>)>;
  using ErrCallback = std::function<void(std::shared_ptr<Session>)>;

  Session(int fd, Callback handler, ErrCallback errHandler);
  ~Session();

  virtual void handle_request() = 0;
  virtual void handle_close() = 0;
  virtual void on_response(const std::string &) = 0;
  int fd() const;

protected:
  int fd_;
  Callback handler_;
  ErrCallback errHandler_;
};
} // namespace server
} // namespace queue_system