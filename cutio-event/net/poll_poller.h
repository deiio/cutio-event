/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_POLL_POLLER_H_
#define CUTIO_EVENT_NET_POLL_POLLER_H_

#include <map>
#include <vector>

#include <poll.h>

#include <cutio-event/net/poller.h>

namespace cutio {
namespace event {

/*
 * IO multiplexing with Poll(2).
 */
class PollPoller : public Poller {
 public:
  ~PollPoller() override = default;

  void Poll(int timeout_ms, ChannelList* active_channels) override;
  void UpdateChannel(Channel* channel) override;
  void RemoveChannel(Channel* channel) override;

 private:
  void FillActiveChannels(int num_events, ChannelList* active_channels) const;

 private:
  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  PollFdList poll_fds_;
  ChannelMap channels_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_POLL_POLLER_H_