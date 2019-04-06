cmake_minimum_required(VERSION 3.13)
project(jwtgen)
set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# The Lean Mean C++ command line arguments parser
# https://sourceforge.net/projects/optionparser/
include_directories(${CMAKE_CURRENT_LIST_DIR}/dependencies/optionparser)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SRC_FILES ${SRC_DIR}/main.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

include(FindPkgConfig)

# OpenSSL (see FindEmbeddedOpenSSL.cmake for more details)
find_package(EmbeddedOpenSSL REQUIRED)
include_directories(${EmbeddedOpenSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${EmbeddedOpenSSL_LIB})

# Link cross-platform clipboard library
find_package(EmbeddedClipboard REQUIRED)
include_directories(${EmbeddedClipboard_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${EmbeddedClipboard_LIB})

# jwt-cpp (cross-platform, header-only)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/jwt-cpp/include)
