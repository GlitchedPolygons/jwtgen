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

# Cross-Platform Clipboard Library
include_directories(${CMAKE_SOURCE_DIR}/dependencies/clip)

if(WIN32)
	set(CLIP_LIB ${CMAKE_SOURCE_DIR}/dependencies/clip/windows-x86_64/clip.lib)
elseif(UNIX AND NOT APPLE)
	set(CLIP_LIB ${CMAKE_SOURCE_DIR}/dependencies/clip/linux-x86_64/libclip.a)
elseif(APPLE)
	set(CLIP_LIB ${CMAKE_SOURCE_DIR}/dependencies/clip/darwin-x86_64/libclip.a)
else()
	message("ERROR: Clipboard library not found for this platform! Please build jwtgen from either Windows, Linux or Apple macOS (all 64-bit only).")
endif()

target_link_libraries(${PROJECT_NAME} ${CLIP_LIB})

# OpenSSL (see FindEmbeddedOpenSSL.cmake for more details)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

# jwt-cpp (cross-platform, header-only)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/jwt-cpp/include)
