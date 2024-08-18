# Cutio-Event - A lightweight C++ network library for Linux.
# Copyright (c) 2024, Niz(mn).  All rights reserved.
# https://furzoom.com
# https://github.com/deiio/cutio-event
#

add_executable(timestamp_unittest timestamp_unittest.cc)
target_link_libraries(timestamp_unittest cutio-event_base)
