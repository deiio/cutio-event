# Cutio-Event - A lightweight C++ network library for Linux.
# Copyright (c) 2024, Niz(mn).  All rights reserved.
# https://furzoom.com
# https://github.com/deiio/cutio-event
#

add_executable(echo_server_unittest echo_server_unittest.cc)
target_link_libraries(echo_server_unittest cutio-event_net)

add_executable(event_loop_unittest event_loop_unittest.cc)
target_link_libraries(event_loop_unittest cutio-event_net)

add_executable(thread_model_unittest thread_model_unittest.cc)
target_link_libraries(thread_model_unittest cutio-event_net)

add_executable(timer_queue_unittest timer_queue_unittest.cc)
target_link_libraries(timer_queue_unittest cutio-event_net)
