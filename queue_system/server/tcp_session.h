#pragma once
#include "session.h"

#include <functional>
#include <memory>
#include <string>

namespace queue_system {
namespace server {

class TcpSession : public Session {
public:
  TcpSession(int fd, Callback handler);
  bool handle_request() override;
  static std::shared_ptr<Session> make_session(int fd, Callback handler);

private:
  std::string buffer_;
  static constexpr int BUF_SIZE = 1024;
};
} // namespace server
} // namespace queue_system