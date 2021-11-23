project(Queue_Management_System)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

include_directories(shared)

add_subdirectory(server)
add_subdirectory(storage)

add_executable(queue_system main.cpp)

target_link_libraries(queue_system server storage Threads::Threads)