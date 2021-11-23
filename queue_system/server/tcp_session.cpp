#include "tcp_session.h"
#include <sys/socket.h>

namespace queue_system {
namespace server {
TcpSession::TcpSession(int fd, Callback handler)
    : Session(fd, handler), buffer_(BUF_SIZE, '\0') {}
bool TcpSession::handle_request() { 
    auto byteRecv = recv(fd_, buffer_.data(), BUF_SIZE, 0);
    if(byteRecv <= 0) {
      return false;
    } 
    handler_(buffer_);
    return true; 
}

std::shared_ptr<Session> TcpSession::make_session(int fd, Callback handler) {
  return std::make_shared<TcpSession>(fd, handler);
}
} // namespace server
} // namespace queue_system