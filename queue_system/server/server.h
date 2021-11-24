#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "common.h"
#include "session.h"

namespace queue_system {
namespace server {

// forward declaration
class Poll;

class Server {
public:
  enum class SessionType { TCP };

  struct ServerException : public std::runtime_error {
    ServerException(const char *msg) : std::runtime_error(msg) {}
  };
  Server(const char *host, short port, SessionType sessionType);
  ~Server();
  void run(Session::Callback handler);
  void stop();

private:
  using SessionFactory =
      std::function<std::shared_ptr<Session>(int, Session::Callback, Session::ErrCallback)>;
  std::shared_ptr<Poll> poll_;
  int serverFd_{-1};
  std::atomic_bool isRunningServer_{true};
  std::atomic_bool isRunningPoll_{true};
  SessionFactory sessionFactory_;
  int on_accept();
  void poll();
};

} // namespace server
} // namespace queue_system