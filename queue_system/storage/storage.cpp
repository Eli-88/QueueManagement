#include "storage.h"

namespace queue_system {
namespace storage {
bool Storage::add_organization(const std::string &companyName,
                               size_t servingQueueSize) {
  allWaitingQueues_.find(companyName);
  if (allWaitingQueues_.find(companyName) != allWaitingQueues_.end()) {
    return false;
  }
  allWaitingQueues_[companyName] = std::make_unique<WaitingQueue>();
  allServingQueues_[companyName] =
      std::make_unique<ServingQueue>(servingQueueSize);
  return true;
}
bool Storage::insert_waiting(const std::string &companyName, const Data &data) {
  auto waitingQueueIter = allWaitingQueues_.find(companyName);
  auto servingQueueIter = allServingQueues_.find(companyName);
  if (waitingQueueIter == allWaitingQueues_.end() or
      servingQueueIter == allServingQueues_.end()) {
    return false;
  }
  auto &waitingQueue = waitingQueueIter->second;
  auto &servingQueue = servingQueueIter->second;

  waitingQueue->insert(data);

  // keep inserting to serving queue until it is full or waiting queue is empty
  while (not waitingQueue->empty()) {
    auto d = waitingQueue->front();
    waitingQueue->pop();
    if (not servingQueue->insert(d.value())) {
      return false;
    }
  }
  return true;
}

std::optional<Data> Storage::pop_serving(const std::string &companyName) {
  auto servingQueueIter = allServingQueues_.find(companyName);
  auto waitingQueueIter = allWaitingQueues_.find(companyName);

  if (servingQueueIter == allServingQueues_.end() or
      waitingQueueIter == allWaitingQueues_.end()) {
    return {};
  }
  auto &servingQueue = servingQueueIter->second;
  auto &waitingQueue = waitingQueueIter->second;

  while (not waitingQueue->empty()) {
    const auto pendingData = waitingQueue->front();
    if (pendingData) {
      if (not servingQueue->insert(pendingData.value())) {
        break;
      }
      waitingQueue->pop();
    }
  }
  const auto result = servingQueue->pop();
  return result;
  return {};
}

std::vector<Data> Storage::display_all_serving(const std::string &companyName) {
  auto servingQueue = allServingQueues_.find(companyName);
  if (servingQueue == allServingQueues_.end()) {
    return {};
  }
  return servingQueue->second->display_all();
}

} // namespace storage
} // namespace queue_system