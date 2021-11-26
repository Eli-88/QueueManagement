#include "common.h"
#include <memory>

namespace queue_system {
DetachableThread::DetachableThread(const char *name,
                                   const std::function<void()> &impl) {
  thread_ = std::move(std::thread([&] {
    pthread_setname_np(name);
    impl();
  }));
  thread_.detach();
}
} // namespace queue_system