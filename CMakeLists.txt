cmake_minimum_required(VERSION 3.20)
project(utils)

set(CMAKE_CXX_STANDARD 14)

add_executable(utils main.cpp base/directory.cpp base/file.cpp)
