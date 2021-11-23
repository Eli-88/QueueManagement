#pragma once

#include <string>

namespace queue_system {
namespace protocol {
class BaseMsg {
public:
  BaseMsg(const std::string &companyName);

protected:
  std::string companyName_;
};
} // namespace protocol
} // namespace queue_system