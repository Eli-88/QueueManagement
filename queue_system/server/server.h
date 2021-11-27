#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>

#include "common.h"
#include "session.h"

namespace queue_system {
namespace server {

// forward declaration
class Poll;

class Server {
public:
  enum class SessionType { TCP };
  Server(const char *host, short port, SessionType sessionType);
  ~Server();
  void run(Session::Callback handler);
  void stop();

private:
  using SessionPtr = std::shared_ptr<Session>;
  using SessionFactory =
      std::function<SessionPtr(int, Session::Callback, Session::ErrCallback)>;
  std::unique_ptr<Poll> poll_;
  int serverFd_{-1};
  std::atomic_bool isRunningServer_{true};
  std::atomic_bool isRunningPoll_{true};
  SessionFactory sessionFactory_;
  std::unordered_map<int, SessionPtr> allSessions_; 
  int on_accept();
};

} // namespace server
} // namespace queue_system