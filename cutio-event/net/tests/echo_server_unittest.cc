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
using namespace std::placeholders;

class EchoServer {
 public:
  EchoServer(EventLoop* loop, const InetAddress& listen_addr)
    : loop_(loop),
      server_(loop, listen_addr) {
    server_.SetConnectionCallback(std::bind(&EchoServer::OnConnection, this, _1));
    server_.SetReadCallback(std::bind(&EchoServer::OnMessage, this, _1, _2, _3));
  }

  void Start() {
    server_.Start();
  }

 private:
  void OnConnection(TcpConnection* conn) {

  }

  void OnMessage(TcpConnection*, const void* buf, ssize_t len) {}

 private:
  EventLoop* loop_;
  TcpServer server_;
};

int main() {
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop loop;
  InetAddress listen_addr(9527);
  EchoServer server(&loop, listen_addr);
  server.Start();

  loop.Loop();

  return 0;
}
