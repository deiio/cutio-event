# Cutio-Event - A lightweight C++ network library for Linux.
# Copyright (c) 2024, Niz(mn).  All rights reserved.
# https://furzoom.com
# https://github.com/deiio/cutio-event
#

set(net_SRCS
    acceptor.cc
    channel.cc
    channel_buffer.cc
    default_poller.cc
    epoll_poller.cc
    event_loop.cc
    inet_address.cc
    poll_poller.cc
    poller.cc
    socket.cc
    sockets_ops.cc
    tcp_connection.cc
    tcp_server.cc
    thread_model.cc
    timer.cc
    timer_queue.cc
)

add_library(cutio-event_net ${net_SRCS})
target_link_libraries(cutio-event_net cutio-event_base)

add_subdirectory(tests)