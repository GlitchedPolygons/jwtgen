cmake_minimum_required(VERSION 3.13)
project(jwtgen)
set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# The Lean Mean C++ command line arguments parser
# https://sourceforge.net/projects/optionparser/
include_directories(${CMAKE_CURRENT_LIST_DIR}/dependencies/optionparser)

set(SRC_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
