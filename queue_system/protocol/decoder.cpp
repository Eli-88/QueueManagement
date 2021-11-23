#include "decoder.h"

#include <nlohmann/json.hpp>

namespace queue_system {
namespace protocol {

static ProtocolType decode_signup_json(const nlohmann::json &j) {
  const auto companyName = j.find("company");
  if (companyName != j.end()) {
    return SignUp(companyName->get<std::string>());
  }
  return std::monostate{};
}

static ProtocolType decode_add_queue(const nlohmann::json &j) {
  const auto companyName = j.find("company");
  if (companyName != j.end()) {
    const auto id = j.find("id");
    if (id != j.end()) {
      return AddQueue(companyName->get<std::string>(), Data{id->get<int>()});
    }
  }
  return std::monostate{};
}

static ProtocolType decode_pop_queue(const nlohmann::json &j) {
  const auto companyName = j.find("company");
  if (companyName != j.end()) {
    return PopQueue(companyName->get<std::string>());
  }
  return std::monostate{};
}

ProtocolType JsonDecoder::decode(const std::string &rawMsg) {
  try {
    const auto decoded = nlohmann::json::parse(rawMsg);
    const auto msgType = decoded.find("msg_type");

    do {
      if (msgType == decoded.end())
        break;
      if (not msgType->is_number_integer())
        break;
      switch (msgType->get<int>()) {
      case MsgType::SIGN_UP:
        return decode_signup_json(decoded);
      case MsgType::ADD_QUEUE:
        return decode_add_queue(decoded);
      case MsgType::POP_QUEUE:
        return decode_pop_queue(decoded);
      default:
        break;
      }
    } while (false);

  } catch (...) {
  }
  return {};
}
} // namespace protocol
} // namespace queue_system