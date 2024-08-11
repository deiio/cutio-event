// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/epoll_poller.h>

namespace cutio {
namespace event {
namespace {

// On Linux, the constants of poll(2) and epoll(4) are
// expected to be the same.

}  // anonymous namespace

void EPollPoller::Poll(int timeout_ms, Poller::ChannelList* active_channels) {

}

void EPollPoller::UpdateChannel(Channel* channel) {

}

}  // namespace event
}  // namespace cutio
