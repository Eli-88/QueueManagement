#pragma once
#include "session.h"

#include <functional>
#include <memory>
#include <string>

namespace queue_system {
namespace server {

class TcpSession : public Session {
public:
  TcpSession(int fd, Callback handler, ErrCallback errHandler);
  void handle_request() override;
  void on_response(const std::string &reply) override;
  void handle_close() override;
  static std::shared_ptr<Session> make_session(int fd, Callback handler,
                                               ErrCallback errHandler);

private:
  std::string buffer_;
  static constexpr int BUF_SIZE = 1024;
};
} // namespace server
} // namespace queue_system