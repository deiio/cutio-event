// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/channel.h>

#include <poll.h>

namespace cutio {
namespace event {

const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1) {}

Channel::~Channel() {}

void Channel::HandleEvent() {
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
    if (close_callback_) close_callback_();
  }
  if (revents_ & POLLNVAL) {
    perror("Channel::HandleEvent() POLLNVAL");
  }
  if (revents_ & (POLLERR | POLLNVAL)) {
    if (error_callback_) error_callback_();
  }
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
    if (read_callback_) read_callback_();
  }
  if (revents_ & (POLLOUT)) {
    if (write_callback_) write_callback_();
  }
}

}  // namespace event
}  // namespace cutio
