#pragma once

#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>
#include "session_observer.h"

namespace queue_system {

namespace server {
class Server;
class Session;
} // namespace server

namespace protocol {
class SignUp;
class AddQueue;
class PopQueue;
class ConnectDisplay;
} // namespace protocol

namespace storage {
class Storage;
}

class QueueManager: public ISessionObserver {
public:
  using SessionPtr = std::shared_ptr<server::Session>;
  QueueManager(const char *host, short port);
  void run();
  void on_request(const std::string &, std::shared_ptr<server::Session>);

  void on_sign_up(const protocol::SignUp &, SessionPtr);
  void on_add_queue(const protocol::AddQueue &, SessionPtr);
  void on_pop_queue(const protocol::PopQueue &, SessionPtr);
  void on_connect_display(const protocol::ConnectDisplay &, SessionPtr);
  void on_invalid(const std::monostate &, SessionPtr);
  void on_session_close(SessionPtr) override;

private:
  std::shared_ptr<storage::Storage> storage_;
  std::shared_ptr<server::Server> server_;
  std::unordered_map<std::string, std::set<SessionPtr>> allDisplayConn_;
  std::unordered_map<SessionPtr, std::string> sessionMapping_;
  void update_display(const std::string &companyName);
};
} // namespace queue_system