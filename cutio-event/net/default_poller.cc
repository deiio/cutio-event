// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

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
