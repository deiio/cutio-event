# Copyright (c) 2024 Furzoom.com. All rights reserved.
# Author: mn@furzoom.com
#
# project cutio-event.
#

add_executable(timestamp_unittest timestamp_unittest.cc)
target_link_libraries(timestamp_unittest cutio-event_base)
