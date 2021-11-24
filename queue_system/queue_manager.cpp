#include "queue_manager.h"
#include "common.h"
#include "protocol/decoder.h"
#include "server/server.h"
#include "storage/storage.h"
#include <future>

namespace queue_system {

QueueManager::QueueManager(const char *host, short port)
    : server_(std::make_shared<server::Server>(
          host, port, server::Server::SessionType::TCP)),
      storage_(std::make_shared<storage::Storage>()) {}

void QueueManager::run() {
  std::packaged_task<void()> task([this] {
    server_->run([this](const std::string &req,
                        std::shared_ptr<server::Session> session) {
      on_request(req, session);
    });
  });
  auto f = task.get_future();
  serverThread_ = std::move(std::thread(std::move(task)));
  f.get();
}

void QueueManager::on_request(const std::string &req,
                              std::shared_ptr<server::Session> session) {
  auto msg = protocol::JsonDecoder::decode(req);
  std::visit(overloaded{[this, session](const protocol::SignUp &msg) {
                          on_sign_up(msg, session);
                        },
                        [this, session](const protocol::AddQueue &msg) {
                          on_add_queue(msg, session);
                        },
                        [this, session](const protocol::PopQueue &msg) {
                          on_pop_queue(msg, session);
                        },
                        [this, session](const protocol::ConnectDisplay &msg) {
                          on_connect_display(msg, session);
                        },
                        [this, session](const std::monostate &msg) {
                          on_invalid(msg, session);
                        }},
             msg);
}

void QueueManager::wait() { serverThread_.join(); }

void QueueManager::on_sign_up(const protocol::SignUp &, SessionPtr) {}
void QueueManager::on_add_queue(const protocol::AddQueue &, SessionPtr) {}
void QueueManager::on_pop_queue(const protocol::PopQueue &, SessionPtr) {}
void QueueManager::on_connect_display(
    const protocol::ConnectDisplay &msg,
    std::shared_ptr<server::Session> session) {
  auto displayConn = allDisplayConn_.find(msg.company_name());
  if (displayConn != allDisplayConn_.end()) {
    displayConn->second.insert(session);
  }
}
void QueueManager::on_invalid(const std::monostate &, SessionPtr session) {
  session->handle_close();
}

} // namespace queue_system