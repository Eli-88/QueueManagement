#pragma once

#include <memory>

namespace queue_system {

namespace server {
class Session;
}

class ISessionObserver {
public:
  virtual void on_session_close(std::shared_ptr<server::Session>) = 0;
};
} // namespace queue_system