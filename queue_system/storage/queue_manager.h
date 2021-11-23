#pragma once

#include "queue_type.h"
#include <unordered_map>

namespace queue_system {
namespace storage {
class QueueManager {
  bool addOrganization(const std::string &companyName, size_t servingQueueSize);
  bool insert_waiting(const std::string &companyName, const Data &data);
  std::optional<Data> pop_serving(const std::string &companyName);
  std::vector<Data> display_all_serving(const std::string &companyName);

private:
  std::unordered_map<std::string, std::unique_ptr<ServingQueue>>
      allServingQueues_;
  std::unordered_map<std::string, std::unique_ptr<WaitingQueue>>
      allWaitingQueues_;
};
} // namespace storage
} // namespace queue_system