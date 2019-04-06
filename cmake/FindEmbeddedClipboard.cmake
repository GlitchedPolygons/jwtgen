if (CYGWIN)
  set(EmbeddedClipboard_DIR ${CMAKE_SOURCE_DIR}/dependencies/libclipboard/cygwin-x86_64)
elseif (WIN32)
  set(EmbeddedClipboard_DIR ${CMAKE_SOURCE_DIR}/dependencies/libclipboard/windows-x86_64)
elseif (UNIX AND NOT APPLE)
  set(EmbeddedClipboard_DIR ${CMAKE_SOURCE_DIR}/dependencies/libclipboard/linux-x86_64)
elseif (APPLE)
  set(EmbeddedClipboard_DIR ${CMAKE_SOURCE_DIR}/dependencies/libclipboard/darwin-x86_64)
endif ()

find_library(
        EmbeddedClipboard_LIB
        NAMES clipboard libclipboard
        PATHS ${EmbeddedClipboard_DIR}/lib)

if (EmbeddedClipboard_LIB)
  message(FindEmbeddedClipboard.cmake:\ Found\ libclipboard\ Path:\ ${EmbeddedClipboard_LIB})
else ()
  message("ERROR: Couldn't find libclipboard library dependency!")
endif ()

set(EmbeddedClipboard_INCLUDE_DIR ${EmbeddedClipboard_DIR}/include)
