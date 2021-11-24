#include "base_message.h"

namespace queue_system {
namespace protocol {
BaseMsg::BaseMsg(const std::string &companyName) : companyName_(companyName) {}

const std::string &BaseMsg::company_name() const { return companyName_; }
} // namespace protocol
} // namespace queue_system