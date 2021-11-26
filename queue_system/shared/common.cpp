#include "common.h"
#include <memory>

namespace queue_system {
Thread::Thread(const std::function<void()> &impl) {
  std::packaged_task<void()> task(impl);
  future_ = task.get_future();
  thread_ = std::move(std::thread(std::move(task)));
}

void Thread::join() {
  future_.get();
  thread_.join();
}
} // namespace queue_system