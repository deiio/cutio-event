/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/tcp_server.h>

#include <assert.h>

#include <functional>
#include <utility>

#include <cutio-event/net/acceptor.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/inet_address.h>
#include <cutio-event/net/sockets_ops.h>
#include <cutio-event/net/tcp_connection.h>
#include <cutio-event/net/thread_model.h>

using namespace std::placeholders;

namespace cutio {
namespace event {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listen_addr)
  : TcpServer(loop, listen_addr, listen_addr.ToHostPort()) {}

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listen_addr, string name)
  : loop_(loop),
    name_(std::move(name)),
    acceptor_(new Acceptor(loop, listen_addr)),
    thread_model_(new ThreadModel(loop)),
    started_(false),
    next_conn_id_(1) {
  acceptor_->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection, this, _1, _2));
}

TcpServer::~TcpServer() {

}

void TcpServer::SetThreadNum(int num_threads) {
  assert(0 <= num_threads);
  thread_model_->SetThreadNum(num_threads);
}

void TcpServer::Start() {
  if (!started_) {
    started_ = true;
    thread_model_->Start();
  }

  if (!acceptor_->Listening()) {
    acceptor_->Listen();
  }
}

void TcpServer::NewConnection(int sockfd, const InetAddress& peer_addr) {
  loop_->AssertInLoopThread();
  EventLoop* io_loop = thread_model_->GetNextLoop();
  char buf[32];
  snprintf(buf, sizeof(buf), "%s#%d", name_.c_str(), next_conn_id_);
  ++next_conn_id_;
  string conn_name = server_name_ + buf;

  InetAddress local_addr(sockets::GetLocalAddr(sockfd));
  TcpConnectionPtr conn(new TcpConnection(loop_, conn_name, sockfd, local_addr, peer_addr));
  connections_[conn_name] = conn;
  conn->SetConnectionCallback(connection_cb_);
  conn->SetMessageCallback(message_cb_);
  conn->SetCloseCallback(std::bind(&TcpServer::RemoveConnection, this, _1));
  io_loop->RunInLoop([conn] { conn->ConnectEstablished(); });
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& conn) {
  loop_->RunInLoop([this, conn] { RemoveConnectionInLoop(conn); });
}

void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& conn) {
  loop_->AssertInLoopThread();
  size_t n = connections_.erase(conn->Name());
  assert(n == 1);
  auto* io_loop = conn->GetLoop();
  io_loop->RunInLoop([conn] { conn->ConnectDestroyed(); });
}

}  // namespace event
}  // namespace cutio
