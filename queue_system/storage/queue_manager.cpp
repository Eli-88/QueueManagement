#include "queue_manager.h"

namespace queue_system {
namespace storage {
bool QueueManager::addOrganization(const std::string &companyName,
                                   size_t servingQueueSize) {
  if (allWaitingQueues_.find(companyName) == allWaitingQueues_.end()) {
    return false;
  }
  allWaitingQueues_[companyName] = std::make_unique<WaitingQueue>();
  allServingQueues_[companyName] =
      std::make_unique<ServingQueue>(servingQueueSize);
  return true;
}
bool QueueManager::insert_waiting(const std::string &companyName,
                                  const Data &data) {
  auto waitingQueue = allWaitingQueues_.find(companyName);
  if (waitingQueue == allWaitingQueues_.end()) {
    return false;
  }
  waitingQueue->second->insert(data);
  allServingQueues_[companyName]->insert(data);
  return true;
}

std::optional<Data> QueueManager::pop_serving(const std::string &companyName) {
  auto servingQueue = allServingQueues_.find(companyName);
  if (servingQueue == allServingQueues_.end()) {
    return {};
  }
  const auto result = servingQueue->second->pop();
  auto &waitingQueue = allWaitingQueues_[companyName];
  while (not allWaitingQueues_.empty()) {
    const auto pendingData = waitingQueue->front();
    if (pendingData) {
      if (not servingQueue->second->insert(pendingData.value())) {
        break;
      }
      waitingQueue->pop();
    }
  }
  return result;
}

std::vector<Data>
QueueManager::display_all_serving(const std::string &companyName) {
  auto servingQueue = allServingQueues_.find(companyName);
  if (servingQueue == allServingQueues_.end()) {
    return {};
  }
  return servingQueue->second->display_all();
}

} // namespace storage
} // namespace queue_system