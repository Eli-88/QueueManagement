#include "message.h"

namespace queue_system {
namespace protocol {
SignUp::SignUp(const std::string &companyName) : BaseMsg(companyName) {}

MsgType SignUp::msg_type() { return MsgType::SIGN_UP; }

AddQueue::AddQueue(const std::string &companyName, const Data &data)
    : BaseMsg(companyName), data_(data) {}
const Data &AddQueue::data() const { return data_; }

MsgType AddQueue::msg_type() { return MsgType::ADD_QUEUE; }

PopQueue::PopQueue(const std::string &companyName) : BaseMsg(companyName) {}

MsgType PopQueue::msg_type() { return MsgType::POP_QUEUE; }

MsgType ConnectDisplay::msg_type() { return MsgType::CONNECT_DISPLAY; }

ConnectDisplay::ConnectDisplay(const std::string &companyName)
    : BaseMsg(companyName) {}

} // namespace protocol
} // namespace queue_system