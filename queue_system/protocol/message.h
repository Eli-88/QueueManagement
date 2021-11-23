#pragma once

#include "constant.h"
#include "base_message.h"
#include "data.h"

#include <string>
#include <string_view>

namespace queue_system {
namespace protocol {
class SignUp : public BaseMsg {
public:
  static MsgType msg_type();
  SignUp(const std::string &companyName);
};

class AddQueue : public BaseMsg {
public:
  static MsgType msg_type();
  AddQueue(const std::string &companyName, const Data &data);
  const Data &data() const;

private:
  Data data_;
};

class PopQueue : public BaseMsg {
public:
  static MsgType msg_type();
  PopQueue(const std::string &companyName);
};
} // namespace protocol
} // namespace queue_system