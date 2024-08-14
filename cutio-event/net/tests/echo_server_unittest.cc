// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/acceptor.h>
#include <cutio-event/net/inet_address.h>
#include <cutio-event/net/tcp_server.h>

using namespace cutio::event;

class EchoServer {
 public:
  EchoServer(EventLoop* loop, const InetAddress& listen_addr)
    : loop_(loop),
      server_(loop, listen_addr) {}

  void Start() {
    server_.Start();
  }

 private:
  EventLoop* loop_;
  TcpServer server_;
};

int main() {
  EventLoop loop;
  InetAddress listen_addr(9527);
  EchoServer server(&loop, listen_addr);
  server.Start();

  loop.Loop();

  return 0;
}
