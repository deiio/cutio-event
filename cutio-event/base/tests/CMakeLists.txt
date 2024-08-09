# Copyright (c) 2024 Furzoom.com. All rights reserved.
# Author: mn@furzoom.com
#
# project cutio-event.
#

add_executable(utc_time_unittest utc_time_unittest.cc)
target_link_libraries(utc_time_unittest cutio-event_base)
