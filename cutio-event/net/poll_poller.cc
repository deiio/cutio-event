// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/poll_poller.h>

#include <assert.h>

#include <cutio-event/net/channel.h>

namespace cutio {
namespace event {

void PollPoller::Poll(int timeout_ms, ChannelList* active_channels) {
  int num_events = ::poll(&*poll_fds_.begin(), poll_fds_.size(), timeout_ms);

  for (auto pfd = poll_fds_.begin(); pfd != poll_fds_.end() && num_events > 0; ++pfd) {
    if (pfd->revents > 0) {
      --num_events;
      Channel* channel = channels_[pfd->fd];
      assert(channel->Fd() == pfd->fd);
      channel->SetREvents(pfd->revents);
      active_channels->push_back(channel);
    }
  }
}

void PollPoller::UpdateChannel(Channel* channel) {

}

}  // namespace event
}  // namespace cutio
