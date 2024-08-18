/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
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
