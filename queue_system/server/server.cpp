#include "server.h"
#include "poll.h"
#include "tcp_session.h"

#include <arpa/inet.h>
#include <exception>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

namespace queue_system {
namespace server {

namespace {
template <typename... Args>
void check_sock_err(int e, const char *fmt, Args &&...args) {
  check_and_throw<ServerException>((e != -1), fmt, std::forward<Args>(args)...);
}

} // namespace

Server::Server(const char *host, short port, Server::SessionType sessionType)
    : poll_(std::make_unique<Poll>()) {
  switch (sessionType) {
  case SessionType::TCP:
    sessionFactory_ = TcpSession::make_session;
    break;
  default:
    check_and_throw<ServerException>(
        false, "session factory init should not reach here");
    break;
  }

  serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
  check_sock_err(serverFd_, "server socket creation fail");

  const int optval = 1;
  auto err =
      setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  check_sock_err(err,
                 "unable to set socket option for SO_REUSEADDR for server");

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(host);
  serverAddr.sin_port = htons((unsigned short)port);
  err = bind(serverFd_, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  check_sock_err(err, "unable to bind server to address[%s:%d]", host, port);

  err = listen(serverFd_, 2048);
  check_sock_err(err, "unable to listen for server");

  poll_->add(serverFd_);
}

Server::~Server() { close(serverFd_); }

void Server::stop() {
  isRunningServer_.store(false);
  isRunningPoll_.store(false);
  close(serverFd_);
}

int Server::on_accept() {
  sockaddr_in remoteAddr;
  socklen_t addrLen = sizeof(remoteAddr);
  auto connFd =
      accept(serverFd_, reinterpret_cast<sockaddr *>(&remoteAddr), &addrLen);
  check_sock_err(connFd, "server accept fail");
  return connFd;
}

void Server::run(Session::Callback handler) {
  while (isRunningServer_) {
    const auto allFds = poll_->poll_once();
    for (const auto fd : allFds) {
      if (fd == serverFd_) {
        // handle new connection
        const auto connFd = on_accept();
        poll_->add(connFd);
        auto session =
            sessionFactory_(connFd, handler, [this](SessionPtr session) {
              poll_->remove(session->fd());
              auto iter = allSessions_.find(session->fd());
              check_and_throw<ServerException>(
                  (iter != allSessions_.end()),
                  "fd[%d] not found, not suppose to happen", session->fd());
              iter->second.reset();
              allSessions_.erase(iter);
            });
        auto sessionIter = allSessions_.find(connFd);
        if (sessionIter != allSessions_.end()) {
          sessionIter->second.reset();
          sessionIter->second = session;
        } else {
          allSessions_[connFd] = session;
        }
      } else {
        // handle connected session
        auto iter = allSessions_.find(fd);
        check_and_throw<ServerException>(
            (iter != allSessions_.end()),
            "file descriptor[%d] not found, not suppose to happen", fd);
        iter->second->handle_request();
      }
    }
  }
}

} // namespace server
} // namespace queue_system