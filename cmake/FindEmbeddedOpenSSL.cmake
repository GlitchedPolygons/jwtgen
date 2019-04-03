if (CYGWIN)
  set(EmbeddedOpenSSL_DIR ${CMAKE_SOURCE_DIR}/dependencies/openssl/cygwin-x86_64)
  find_library(
          EmbeddedOpenSSL_LIB
          NAMES crypto ssl
          PATHS ${EmbeddedOpenSSL_DIR}/lib
          NO_DEFAULT_PATH)
elseif (WIN32)
  set(EmbeddedOpenSSL_DIR ${CMAKE_SOURCE_DIR}/dependencies/openssl/windows-x86_64)
  find_library(
          EmbeddedOpenSSL_LIB
          NAMES libcrypto libssl
          PATHS ${EmbeddedOpenSSL_DIR}/lib
          NO_DEFAULT_PATH)
elseif (UNIX AND NOT APPLE)
  set(EmbeddedOpenSSL_DIR ${CMAKE_SOURCE_DIR}/dependencies/openssl/linux-x86_64)
  find_library(
          EmbeddedOpenSSL_LIB
          NAMES crypto ssl
          PATHS ${EmbeddedOpenSSL_DIR}/lib
          NO_DEFAULT_PATH)
elseif (APPLE)
  set(EmbeddedOpenSSL_DIR ${CMAKE_SOURCE_DIR}/dependencies/openssl/darwin-x86_64)
  find_library(
          EmbeddedOpenSSL_LIB
          NAMES crypto ssl
          PATHS ${EmbeddedOpenSSL_DIR}/lib
          NO_DEFAULT_PATH)
else ()
  message("FindEmbeddedOpenSSL.cmake: No supported embedded OpenSSL library was found for this platform inside the dependencies/openssl folder; attempting to locate OpenSSL lib externally, where the exact version and compatibility with this build pipeline remain a mistery. Brace brace...")
  find_library(
          EmbeddedOpenSSL_LIB
          NAMES crypto ssl libcrypto libssl)
  if (NOT EmbeddedOpenSSL_LIB)
    find_package(OpenSSL REQUIRED)
    if (OPENSSL_FOUND)
      set(EmbeddedOpenSSL_LIB ${OPENSSL_LIBRARIES})
    endif ()
  endif ()
endif ()

message(FindEmbeddedOpenSSL.cmake:\ Found\ OpenSSL\ Path:\ ${EmbeddedOpenSSL_LIB})

set(EmbeddedOpenSSL_INCLUDE_DIR ${EmbeddedOpenSSL_DIR}/include)
