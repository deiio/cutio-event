# Copyright (c) 2024 Furzoom.com. All rights reserved.
# Author: mn@furzoom.com
#
# project cutio-event.
#

set(base_SRCS
    thread.cc
    utc_time.cc
)

add_library(cutio-event_base ${base_SRCS})

add_subdirectory(tests)