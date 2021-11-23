add_library(protocol)
target_sources(protocol PRIVATE
base_message.cpp
message.cpp
decoder.cpp
)