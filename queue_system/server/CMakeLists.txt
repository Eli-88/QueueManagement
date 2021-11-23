add_library(server)

target_sources(server PRIVATE 
poll.cpp 
server.cpp 
session.cpp 
tcp_session.cpp
)