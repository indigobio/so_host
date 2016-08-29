cmake_minimum_required(VERSION 2.8)
project(so_host)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#include_directories("/home/pwinton/ascent/ascent-cpp-compute/compute-core/include")

#include(FindPackageHandleStandardArgs)
#find_package(Protobuf REQUIRED)
#find_package(Erlang REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp erl_comm.cpp)
add_executable(so_host ${SOURCE_FILES})
target_link_libraries(so_host erl_interface ei dl)
