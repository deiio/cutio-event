// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/tcp_server.h>

#include <assert.h>

#include <functional>

#include <cutio-event/net/acceptor.h>
#include <cutio-event/net/thread_model.h>

using namespace std::placeholders;

namespace cutio {
namespace event {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listen_addr)
  : loop_(loop),
    acceptor_(new Acceptor(loop, listen_addr)),
    thread_model_(new ThreadModel(loop)),
    started_(false) {
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

void TcpServer::NewConnection(int fd, const InetAddress& peer_addr) {

}

}  // namespace event
}  // namespace cutio
