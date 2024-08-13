# Copyright (c) 2024 Furzoom.com. All rights reserved.
# Author: mn@furzoom.com
#
# project cutio-event.
#

set(net_SRCS
    acceptor.cc
    channel.cc
    default_poller.cc
    epoll_poller.cc
    event_loop.cc
    inet_address.cc
    poll_poller.cc
    poller.cc
    socket.cc
    sockets_ops.cc
    tcp_server.cc
    timer.cc
    timer_queue.cc
)

add_library(cutio-event_net ${net_SRCS})
target_link_libraries(cutio-event_net cutio-event_base)

add_subdirectory(tests)