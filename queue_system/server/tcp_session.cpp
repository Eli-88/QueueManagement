#include "tcp_session.h"
#include <sys/socket.h>

namespace queue_system {
namespace server {
TcpSession::TcpSession(int fd, Callback handler, ErrCallback errHandler)
    : Session(fd, handler, errHandler), buffer_(BUF_SIZE, '\0') {}

void TcpSession::handle_request() {
  auto byteRecv = recv(fd_, buffer_.data(), BUF_SIZE, 0);
  if (byteRecv <= 0) {
    errHandler_(this->shared_from_this());
    return;
  }
  handler_(buffer_, this->shared_from_this());
}

void TcpSession::on_response(const std::string &reply) {
  const auto byteSend = send(fd_, reply.data(), reply.size(), 0);
  if (byteSend < 0) {
    errHandler_(this->shared_from_this());
  }
}

void TcpSession::handle_close() { 
  errHandler_(this->shared_from_this()); }

std::shared_ptr<Session> TcpSession::make_session(int fd, Callback handler,
                                                  ErrCallback errHandler) {
  return std::make_shared<TcpSession>(fd, handler, errHandler);
}
} // namespace server
} // namespace queue_system