cmake_minimum_required(VERSION 3.13)
project(jwtgen)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_executable(jwtgen main.cpp)