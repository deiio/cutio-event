// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/epoll_poller.h>

#include <poll.h>
#include <sys/epoll.h>

namespace cutio {
namespace event {

// On Linux, the constants of poll(2) and epoll(4) are
// expected to be the same.
static_assert(EPOLLIN == POLLIN, "EPOLLIN != POLLIN");
static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI != POLLPRI");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT != POLLOUT");
static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP != POLLRDHUP");
static_assert(EPOLLERR == POLLERR, "EPOLLERR != POLLERR");
static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP != POLLHUP");

void EPollPoller::Poll(int timeout_ms, Poller::ChannelList* active_channels) {

}

void EPollPoller::UpdateChannel(Channel* channel) {

}

}  // namespace event
}  // namespace cutio
