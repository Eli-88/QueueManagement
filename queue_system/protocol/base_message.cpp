#include "base_message.h"

namespace queue_system {
namespace protocol {
BaseMsg::BaseMsg(const std::string &companyName) : companyName_(companyName) {}
} // namespace protocol
} // namespace queue_system