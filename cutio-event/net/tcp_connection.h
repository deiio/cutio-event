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

#ifndef CUTIO_EVENT_NET_TCP_CONNECTION_H_
#define CUTIO_EVENT_NET_TCP_CONNECTION_H_

#include <memory>
#include <utility>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/types.h>
#include <cutio-event/net/callbacks.h>
#include <cutio-event/net/inet_address.h>

namespace cutio {
namespace event {

class Channel;
class EventLoop;
class Socket;

/**
 * TCP connection, for both client and server usage.
 *
 * This is an interface class, so don't expose too much details.
 */
class TcpConnection : public std::enable_shared_from_this<TcpConnection>,
                      noncopyable {
 public:
  /**
   * Constructs a TcpConnection with a connected sockfd.
   */
  TcpConnection(string name, EventLoop* loop, int sockfd,
                const InetAddress& local_addr, const InetAddress& peer_addr);

  const InetAddress& LocalAddr() const { return local_addr_; }
  const InetAddress& PeerAddr() const { return peer_addr_; }

  void SetConnectionCallback(ConnectionCallback cb) { connection_cb_ = std::move(cb); }
  void SetMessageCallback(MessageCallback cb) { message_cb_ = std::move(cb); }
  void SetCloseCallback(ConnectionCallback cb) { close_cb_ = std::move(cb); }

 private:
  string name_;
  EventLoop* loop_;
  // We don't expose those class to client.
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  InetAddress local_addr_;
  InetAddress peer_addr_;
  ConnectionCallback connection_cb_;
  MessageCallback message_cb_;
  ConnectionCallback close_cb_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TCP_CONNECTION_H_
