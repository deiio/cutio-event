// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/acceptor.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/inet_address.h>
#include <cutio-event/net/sockets_ops.h>

namespace cutio {
namespace event {

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listen_addr)
  : loop_(loop),
    accept_socket_(sockets::CreateNonblockingOrDie()),
    accept_channel_(loop, accept_socket_.Fd()),
    listening_(false) {
  accept_socket_.SetReuseAddr(true);
  accept_socket_.BindAddress(listen_addr);
  accept_channel_.SetReadCallback([this] { Accept(); });
}

void Acceptor::Listen() {
  listening_ = true;
  accept_socket_.Listen();
  accept_channel_.SetEvents(Channel::kReadEvent);
  loop_->UpdateChannel(&accept_channel_);
}

void Acceptor::Accept() {
  InetAddress peer_addr(0);
  int conn_fd = accept_socket_.Accept(&peer_addr);
  auto host_port = peer_addr.ToHostPort();
  LOG_INFO << "connecting from " << host_port;

  if (new_connection_cb_) {
    new_connection_cb_(conn_fd, peer_addr);
  } else {
    sockets::Close(conn_fd);
  }
}

}  // namespace event
}  // namespace cutio
