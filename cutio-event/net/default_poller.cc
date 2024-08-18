/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/epoll_poller.h>
#include <cutio-event/net/poll_poller.h>
#include <cutio-event/net/poller.h>

namespace cutio {
namespace event {

Poller* Poller::NewDefaultPoller() {
  return new PollPoller;
}

}  // namespace event
}  // namespace cutio
