// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

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

 private:
  typedef std::vector<struct pollfd> PollFdList;
  PollFdList poll_fds_;
  std::map<int, Channel*> channels_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_POLL_POLLER_H_