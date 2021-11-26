#include "common.h"
#include "poll.h"
#include "queue_manager.h"
#include <memory>

using namespace queue_system;

int main(int argc, char const *argv[]) {
  if(argc < 2) {
    printf("missing program arguement <host> <port>: i.e. ./queue_system 127.0.0.1 5000\n");
    exit(-1);
  }
  const char* host = argv[1];
  const short port = std::stoi(argv[2]);
  auto queueManagerPtr_ = std::make_unique<QueueManager>(host, port);
  queueManagerPtr_->run();
  return 0;
}
