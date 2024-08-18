# Cutio-Event - A lightweight C++ network library for Linux.
# Copyright (c) 2024, Niz(mn).  All rights reserved.
# https://furzoom.com
# https://github.com/deiio/cutio-event
#

cmake_minimum_required(VERSION 2.9)

project(cutio-event CXX)

set(CXX_FLAGS
    -g
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wold-style-cast
    -MMD
    -Wpointer-arith
    -Wshadow
    -Wno-unused-parameter
    -Wwrite-strings
    -rdynamic
    -lpthread
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(BUILD_TYPE "DEBUG")
if(CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
endif()
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(cutio-event/base)
add_subdirectory(cutio-event/net)
