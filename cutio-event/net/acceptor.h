/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

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
  typedef std::function<void(int fd, const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listen_addr);
  ~Acceptor() = default;

  void SetNewConnectionCallback(const NewConnectionCallback& cb) { new_connection_cb_ = cb; }
  bool Listening() const { return listening_; }
  void Listen();

 private:
  void Accept();

 private:
  EventLoop* loop_;
  Socket accept_socket_;
  Channel accept_channel_;
  NewConnectionCallback new_connection_cb_;
  bool listening_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_ACCEPTOR_H_
