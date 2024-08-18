/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_POLLER_H_
#define CUTIO_EVENT_NET_POLLER_H_

#include <vector>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class Channel;

/**
 * Base class for IO multiplexing.
 *
 * This class doesn't own the Channel objects.
 */
class Poller : noncopyable {
 public:
  typedef std::vector<Channel*> ChannelList;

  virtual ~Poller() = default;

  /**
   * Polls the I/O events.
   * Must be called in the loop thread.
   */
  virtual void Poll(int timeout_ms, ChannelList* active_channels) = 0;

  /**
   * Changes the interested I/O events.
   * Must be called in the loop thread.
   */
  virtual void UpdateChannel(Channel* channel) = 0;

  /**
   *
   */
  virtual void RemoveChannel(Channel* channel) = 0;

  static Poller* NewDefaultPoller();
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_POLLER_H_
