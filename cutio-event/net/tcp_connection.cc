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

#include <cutio-event/net/tcp_connection.h>

#include <utility>

#include <cutio-event/base/logger.h>
#include <cutio-event/net/channel.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/socket.h>
#include <cutio-event/net/sockets_ops.h>

namespace cutio {
namespace event {

TcpConnection::TcpConnection(EventLoop* loop,
                             string name,
                             int sockfd,
                             const InetAddress& local_addr,
                             const InetAddress& peer_addr)
    : loop_(loop),
      name_(std::move(name)),
      state_(kDisconnected),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
      local_addr_(local_addr),
      peer_addr_(peer_addr) {
  channel_->SetReadCallback([this] { HandleRead(); });
  channel_->SetWriteCallback([this] { HandleWrite(); });
  channel_->SetCloseCallback([this] { HandleClose(); });
  channel_->SetErrorCallback([this] { HandleError(); });
  LOG_DEBUG << this << " " << name_ << " ctor";
}

TcpConnection::~TcpConnection() {
  loop_->AssertInLoopThread();
  loop_->RemoveChannel(channel_.get());
  LOG_DEBUG << this << " " << name_ << " dtor";
}

void TcpConnection::Shutdown() {
  sockets::Shutdown(channel_->Fd());
  loop_->RunInLoop([this] { HandleClose(); });
}

void TcpConnection::HandleRead() {
  loop_->AssertInLoopThread();
  int saved_errno;
  auto n = input_buffer_.ReadFd(channel_->Fd(), &saved_errno);
  if (n > 0) {
    message_cb_(shared_from_this(), &input_buffer_);
  } else if (n == 0) {
    HandleClose();
  } else {
    // Check saved_errno
  }
}

void TcpConnection::HandleWrite() {

}

void TcpConnection::HandleClose() {
  loop_->AssertInLoopThread();
  // We don't close fd, leave it to dtor, so we can find leaks easily.
  state_ = kDisconnected;
  channel_->SetEvents(Channel::kNoneEvent);
  loop_->UpdateChannel(channel_.get());

  TcpConnectionPtr guard(shared_from_this());
  connection_cb_(guard);
  // Must be the last line.
  close_cb_(guard);
}

void TcpConnection::HandleError() {

}

void TcpConnection::ConnectEstablished() {
  loop_->AssertInLoopThread();
  state_ = kConnected;
  channel_->Tie(shared_from_this());
  channel_->SetEvents(Channel::kReadEvent);
  loop_->UpdateChannel(channel_.get());

  connection_cb_(shared_from_this());
}

void TcpConnection::ConnectDestroyed() {

}

}  // namespace event
}  // namespace cutio
