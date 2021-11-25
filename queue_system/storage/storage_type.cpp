#include "storage_type.h"

namespace queue_system {
namespace storage {
ServingQueue::ServingQueue(size_t queueSize) : queue_(queueSize) {}

bool ServingQueue::insert(const Data &data) {
  if (size_ == queue_.size()) {
    return false;
  }
  queue_[tail_] = data;
  tail_ = index(tail_ + 1);
  ++size_;
  return true;
}

std::optional<Data> ServingQueue::pop() {
  if (size_ == 0) {
    return {};
  }
  auto const currhead(head_);
  head_ = index(head_ + 1);
  --size_;
  return queue_[currhead];
}

std::vector<Data> ServingQueue::display_all() {
  std::vector<Data> result;
  auto h{head_};
  for (size_t i = 0; i < size_; i++) {
    result.push_back(queue_[h]);
    h = index(h + 1);
  }
  return result;
}

size_t ServingQueue::index(size_t k) const { return k % queue_.size(); }

void WaitingQueue::insert(const Data &data) {
  queue_.push(data);
}

std::optional<Data> WaitingQueue::front() {
  if (queue_.empty()) {
    return {};
  }
  return queue_.front();
}

void WaitingQueue::pop() {
  if (not queue_.empty()) {
    queue_.pop();
  }
}

bool WaitingQueue::empty() const {
  return queue_.empty();
}

} // namespace storage
} // namespace queue_system