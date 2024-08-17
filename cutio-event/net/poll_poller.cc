/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <cutio-event/net/poll_poller.h>

#include <cassert>
#include <cstdio>

#include <cutio-event/base/logger.h>
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
    channel->SetIndex(static_cast<int>(poll_fds_.size() - 1));
    channels_[pfd.fd] = channel;
  } else {
    // Update existing one
    assert(channels_.find(channel->Fd()) != channels_.end());
    assert(channels_[channel->Fd()] == channel);
    int idx = channel->Index();
    assert(0 <= idx && idx < static_cast<int>(poll_fds_.size()));
    auto& pfd = poll_fds_[static_cast<unsigned long>(idx)];
    assert(pfd.fd == channel->Fd() || pfd.fd == -1);
    pfd.events = static_cast<short>(channel->Events());
    if (pfd.events == 0) {
      // Ignore this pollfd
      pfd.fd = -1;
    }
  }
}

}  // namespace event
}  // namespace cutio
