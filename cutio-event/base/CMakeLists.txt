# Cutio-Event - A lightweight C++ network library for Linux.
# Copyright (c) 2024, Niz(mn).  All rights reserved.
# https://furzoom.com
# https://github.com/deiio/cutio-event
#

set(base_SRCS
    logger.cc
    thread.cc
    timestamp.cc
)

add_library(cutio-event_base ${base_SRCS})

add_subdirectory(tests)