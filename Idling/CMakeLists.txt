cmake_minimum_required(VERSION 3.21)
project(Idling)

set(CMAKE_CXX_STANDARD 17)

add_executable(Idling main.cpp)

target_link_libraries(Idling uv)