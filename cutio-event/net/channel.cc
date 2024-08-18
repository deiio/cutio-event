/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/channel.h>

#include <poll.h>

#include <cutio-event/base/logger.h>

namespace cutio {
namespace event {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1),
    tied_(false) {}

Channel::~Channel() {}

void Channel::Tie(const std::shared_ptr<void>& obj) {
  tie_ = obj;
  tied_ = true;
}

void Channel::HandleEvent() {
  std::shared_ptr<void> guard;
  if (tied_) {
    guard = tie_.lock();
    if (guard) {
      HandleEventWithGuard();
    }
  } else {
    HandleEventWithGuard();
  }
}

void Channel::HandleEventWithGuard() {
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
    LOG_INFO << "Channel::HandleEvent() POLLHUP";
    if (close_callback_) close_callback_();
  }
  if (revents_ & POLLNVAL) {
    LOG_WARN << "Channel::HandleEvent() POLLNVAL";
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
