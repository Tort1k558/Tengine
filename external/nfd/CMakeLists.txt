cmake_policy(VERSION 3.2)

project(nfd)

set(SOURCES nfd/src/nfd_common.c nfd/src/include/nfd.h)

if (WIN32)
    list(APPEND SOURCES nfd/src/nfd_win.cpp)
elseif (APPLE)
    list(APPEND SOURCES nfd/src/nfd_cocoa.m)
elseif (UNIX)
    list(APPEND SOURCES nfd/src/nfd_gtk.c)
elseif (UNIX)
    message(FATAL_ERROR "Unknown system")
endif ()

add_library(nfd STATIC ${SOURCES})

target_include_directories(nfd PUBLIC nfd/src/include)
