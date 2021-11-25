#include "queue_manager.h"
#include "common.h"
#include "protocol/decoder.h"
#include "server/server.h"
#include "storage/storage.h"
#include <future>
#include <nlohmann/json.hpp>

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

void QueueManager::on_sign_up(const protocol::SignUp &msg, SessionPtr session) {
  if (session) {
    auto response = nlohmann::json();
    if (storage_->add_organization(msg.company_name(), msg.queue_size())) {
      response["result"] = "success";
      allDisplayConn_[msg.company_name()];
    } else {
      response["result"] = "organization already exists";
    }
    session->on_response(response.dump());
    session->handle_close();
  }
}

void QueueManager::on_add_queue(const protocol::AddQueue &msg,
                                SessionPtr session) {
  if (session) {
    auto response = nlohmann::json();
    if (storage_->insert_waiting(msg.company_name(), msg.data())) {
      response["result"] = "success";
    } else {
      response["result"] = "fail to add to queue";
    }
    session->on_response(response.dump());
    session->handle_close();
    update_display(msg.company_name());
  }
}

void QueueManager::on_pop_queue(const protocol::PopQueue &msg,
                                SessionPtr session) {
  if (session) {
    auto response = nlohmann::json();
    auto data = storage_->pop_serving(msg.company_name());
    if (data) {
      auto d = nlohmann::json::array({{"id", data.value().id}});
      response["result"] = "success";
      response["data"] = d;
    } else {
      response["result"] = "fail to pop";
    }
    session->on_response(response.dump());
    session->handle_close();
    update_display(msg.company_name());
  }
}

void QueueManager::on_connect_display(
    const protocol::ConnectDisplay &msg,
    std::shared_ptr<server::Session> session) {
  if (session) {
    auto displayConn = allDisplayConn_.find(msg.company_name());
    auto response = nlohmann::json();
    if (displayConn != allDisplayConn_.end()) {
      displayConn->second.insert(session);
      response["result"] = "success";
      session->on_response(response.dump());
    } else {
      response["result"] = "not found, need to register company first";
      session->on_response(response.dump());
      session->handle_close();
    }
  }
}

void QueueManager::on_invalid(const std::monostate &, SessionPtr session) {
  if (session) {
    auto response = nlohmann::json();
    response["result"] = "invalid request";
    session->on_response(response.dump());
    session->handle_close();
  }
}

void QueueManager::update_display(const std::string &companyName) {
  auto displayConnIter = allDisplayConn_.find(companyName);
  if (displayConnIter != allDisplayConn_.end()) {
    auto &[_, displayConns] = *displayConnIter;
    for (auto conn : displayConns) {
      if (conn) {
        auto allData = storage_->display_all_serving(companyName);
        nlohmann::json jAllData;
        for (auto &data : allData) {
          jAllData.push_back(data.id);
        }
        auto response = nlohmann::json();
        response["result"] = "success";
        response["serving_queue"] = jAllData.dump();
        conn->on_response(response.dump());
      }
    }
  }
}

} // namespace queue_system