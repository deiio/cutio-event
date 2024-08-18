/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/base/types.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/acceptor.h>
#include <cutio-event/net/channel_buffer.h>
#include <cutio-event/net/inet_address.h>
#include <cutio-event/net/tcp_connection.h>
#include <cutio-event/net/tcp_server.h>

using namespace cutio::event;
using namespace std::placeholders;

class EchoServer {
 public:
  EchoServer(EventLoop* loop, const InetAddress& listen_addr)
    : loop_(loop),
      server_(loop, listen_addr) {
    server_.SetConnectionCallback(std::bind(&EchoServer::OnConnection, this, _1));
    server_.SetReadCallback(std::bind(&EchoServer::OnMessage, this, _1, _2));
    server_.SetThreadNum(0);
  }

  void Start() {
    server_.Start();
  }

 private:
  void OnConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << "conn " << conn->PeerAddr().ToHostPort() << " -> "
             << conn->LocalAddr().ToHostPort() << " " << (conn->Connected() ? "UP" : "DOWN");
    if (!first) {
      first = conn;
    }
    if (first == conn && !conn->Connected()) {
      first.reset();
      loop_->Quit();
    }
  }

  void OnMessage(const TcpConnectionPtr& conn, ChannelBuffer* buf) {
    auto msg = buf->RetrieveAsString();
    LOG_INFO << "recv " << msg.size() << " bytes '" << msg << "'";
  }

 private:
  TcpConnectionPtr first;

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
