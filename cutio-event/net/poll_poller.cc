/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/poll_poller.h>

#include <cassert>
#include <cstdio>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/types.h>
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
  if (channel->Index() < 0) {
    // A new one, add to poll_fds_
    assert(channels_.find(channel->Fd()) == channels_.end());
    pollfd pfd{};
    pfd.fd = channel->Fd();
    pfd.events = static_cast<short>(channel->Events());
    pfd.revents = 0;
    poll_fds_.push_back(pfd);
    auto idx = static_cast<int>(poll_fds_.size() - 1);
    channel->SetIndex(idx);
    channels_[pfd.fd] = channel;
  } else {
    // Update existing one
    assert(channels_.find(channel->Fd()) != channels_.end());
    assert(channels_[channel->Fd()] == channel);
    int idx = channel->Index();
    assert(0 <= idx && idx < static_cast<int>(poll_fds_.size()));
    auto& pfd = poll_fds_[static_cast<unsigned long>(idx)];
    assert(pfd.fd == channel->Fd() || pfd.fd == -channel->Fd() - 1);
    pfd.events = static_cast<short>(channel->Events());
    if (pfd.events == 0) {
      // Ignore this pollfd
      pfd.fd = -channel->Fd() - 1;
      LOG_DEBUG << "set pfd.fd=-fd-1 for=" << channel->Fd();
    }
  }
}

void PollPoller::RemoveChannel(Channel* channel) {
  assert(channels_.find(channel->Fd()) != channels_.end());
  assert(channels_[channel->Fd()] == channel);

  int idx = channel->Index();
  assert(idx >= 0 && idx < static_cast<int>(poll_fds_.size()));
  size_t n = channels_.erase(channel->Fd());
  assert(n == 1);
  if (static_cast<size_t>(idx) == poll_fds_.size() - 1) {
    poll_fds_.pop_back();
  } else {
    int channel_at_end = poll_fds_.back().fd;
    std::iter_swap(poll_fds_.begin() + idx, poll_fds_.end() - 1);
    channels_[channel_at_end]->SetIndex(idx);
    poll_fds_.pop_back();
  }
}

}  // namespace event
}  // namespace cutio
