#pragma once
#include <memory>
#include <optional>
#include <queue>
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
  size_t head_{0};
  size_t tail_{0};
  size_t size_{0};
  size_t index(size_t k) const;
};

class WaitingQueue {
public:
  void insert(const Data &data);
  std::optional<Data> front();
  void pop();
  bool empty() const;

private:
  std::queue<Data> queue_;
};
} // namespace storage
} // namespace queue_system