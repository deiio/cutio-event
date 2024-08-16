// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/tcp_server.h>

#include <functional>

#include <cutio-event/net/acceptor.h>

namespace cutio {
namespace event {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listen_addr)
  : loop_(loop),
    acceptor_(new Acceptor(loop, listen_addr)) {
}

TcpServer::~TcpServer() {

}

void TcpServer::Start() {
  if (!acceptor_->Listening()) {
    acceptor_->Listen();
  }
}

void TcpServer::NewConnection(int fd, const InetAddress& peer_addr) {

}

}  // namespace event
}  // namespace cutio
