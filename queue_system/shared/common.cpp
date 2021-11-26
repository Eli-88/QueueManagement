#include "common.h"
#include <memory>

namespace queue_system {
DetachableThread::DetachableThread(const char *name,
                                   const std::function<void()> &impl) {
  thread_ = std::move(std::thread([&] {
#ifdef __APPLE__
    pthread_setname_np(name);
#endif
    impl();
  }));
#ifdef __linux__
  pthread_setname_np(thread_.native_handle(), name);
#endif
  thread_.detach();
}
} // namespace queue_system