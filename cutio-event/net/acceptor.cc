// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/acceptor.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/net/sockets_ops.h>

namespace cutio {
namespace event {

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listen_addr)
  : loop_(loop),
    accept_socket_(sockets::CreateNonblockingOrDie()),
    accept_channel_(loop, accept_socket_.Fd()),
    listening_(false) {}

void Acceptor::Accept() {

}

void Acceptor::Listen() {

}

}  // namespace event
}  // namespace cutio
