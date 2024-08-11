// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_EPOLL_POLLER_H_
#define CUTIO_EVENT_NET_EPOLL_POLLER_H_


#include <map>

#include <cutio-event/net/poller.h>

namespace cutio {
namespace event {

/**
 * IO multiplexing with epoll(4).
 */
class EPollPoller : public Poller {
 public:
  ~EPollPoller() override = default;

  void Poll(int timeout_ms, ChannelList* active_channels) override;
  void UpdateChannel(Channel* channel) override;

 private:
  std::map<int, Channel*> channels_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_EPOLL_POLLER_H_
