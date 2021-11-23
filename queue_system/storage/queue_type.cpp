#include "queue_type.h"

namespace queue_system {
namespace storage {
ServingQueue::ServingQueue(size_t queueSize) : queue_(queueSize) {}

bool ServingQueue::insert(const Data &data) {
  std::shared_lock lock(mux_);
  size_t prevTail = tail_.load(std::memory_order_acquire);
  size_t currTail{};

  do {
    currTail = index(prevTail + 1);
    if (isFull(index(currTail))) {
      return false;
    }
  } while (not tail_.compare_exchange_weak(prevTail, currTail,
                                           std::memory_order_acq_rel,
                                           std::memory_order_relaxed));

  queue_[currTail] = data;
  return true;
}

std::optional<Data> ServingQueue::pop() {
  std::shared_lock lock(mux_);
  size_t prevHead = head_.load();
  size_t currHead{};
  do {
    currHead = index(prevHead + 1);
    if (isEmpty(currHead)) {
      return {};
    }
  } while (not head_.compare_exchange_weak(prevHead, currHead,
                                           std::memory_order_acq_rel,
                                           std::memory_order_relaxed));
  return queue_[currHead];
}

std::vector<Data> ServingQueue::display_all() {
  std::unique_lock lock(mux_);
  auto h = head_.load();
  auto t = tail_.load();
  std::vector<Data> result;
  while (h != t) {
    result.push_back(queue_[h]);
    h = (h + 1) % queue_.size();
  }
  return result;
}

size_t ServingQueue::index(size_t k) const { return k % queue_.size(); }

bool ServingQueue::isFull(size_t tail) const {
  return tail == index(head_.load() + 1);
}

bool ServingQueue::isEmpty(size_t head) const { return tail_.load() == head; }

void WaitingQueue::insert(const Data &data) {
  std::unique_lock lock(mux_);
  queue_.push(data);
}

std::optional<Data> WaitingQueue::front() {
  std::shared_lock lock(mux_);
  if (queue_.empty()) {
    return {};
  }
  return queue_.front();
}

void WaitingQueue::pop() {
  std::unique_lock lock(mux_);
  if (not queue_.empty()) {
    queue_.pop();
  }
}

} // namespace storage
} // namespace queue_system