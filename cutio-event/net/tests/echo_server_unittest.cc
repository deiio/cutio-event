/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
