cmake_minimum_required(VERSION 2.8)
project(so_host)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES so_host.cpp comm.cpp)
add_executable(so_host ${SOURCE_FILES})
target_link_libraries(so_host dl)

