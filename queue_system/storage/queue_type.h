#pragma once
#include <atomic>
#include <memory>
#include <optional>
#include <queue>
#include <shared_mutex>
#include <vector>

#include "data.h"

namespace queue_system {
namespace storage {
class ServingQueue {
public:
  ServingQueue(size_t queueSize);
  bool insert(const Data &data);
  std::optional<Data> pop();
  std::vector<Data> display_all();

private:
  std::vector<Data> queue_;
  std::atomic<size_t> head_{0};
  std::atomic<size_t> tail_{0};
  std::shared_mutex mux_;
  size_t index(size_t k) const;
  bool isFull(size_t tail) const;
  bool isEmpty(size_t head) const;
};

class WaitingQueue {
public:
  void insert(const Data &data);
  std::optional<Data> front();
  void pop();

private:
  std::queue<Data> queue_;
  std::shared_mutex mux_;
};
} // namespace storage
} // namespace queue_system