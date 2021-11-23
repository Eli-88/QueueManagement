#pragma once

namespace queue_system {
namespace protocol {
enum  MsgType : int { SIGN_UP = 0x00, ADD_QUEUE, POP_QUEUE };
}
} // namespace queue_system