// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_ACCEPTOR_H_
#define CUTIO_EVENT_NET_ACCEPTOR_H_

#include <functional>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/net/channel.h>
#include <cutio-event/net/socket.h>

namespace cutio {
namespace event {

class Channel;
class EventLoop;
class InetAddress;

/**
 * Acceptor of incoming TCP connections.
 */
class Acceptor : noncopyable {
 public:
  Acceptor(EventLoop* loop, const InetAddress& listen_addr);
  ~Acceptor() = default;

  void Accept();

  bool Listening() const { return listening_; }
  void Listen();

 private:
  EventLoop* loop_;
  Socket accept_socket_;
  Channel accept_channel_;
  bool listening_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_ACCEPTOR_H_
