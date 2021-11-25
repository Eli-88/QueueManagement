#pragma once

#include "base_message.h"
#include "constant.h"
#include "data.h"

#include <string>
#include <string_view>

namespace queue_system {
namespace protocol {
class SignUp : public BaseMsg {
public:
  static MsgType msg_type();
  SignUp(const std::string &companyName, int queueSize);
  int queue_size() const;

private:
  int queueSize_;
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

class ConnectDisplay : public BaseMsg {
public:
  static MsgType msg_type();
  ConnectDisplay(const std::string &companyName);
};

} // namespace protocol
} // namespace queue_system