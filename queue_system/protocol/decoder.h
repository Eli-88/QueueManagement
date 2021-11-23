#pragma once

#include "message.h"
#include <variant>

namespace queue_system {
namespace protocol {

using ProtocolType = std::variant<std::monostate, SignUp, AddQueue, PopQueue>;

class JsonDecoder {
public:
  static ProtocolType decode(const std::string &rawMsg);
};
} // namespace protocol
} // namespace queue_system