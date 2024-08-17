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

#ifndef CUTIO_EVENT_NET_TCP_SERVER_H_
#define CUTIO_EVENT_NET_TCP_SERVER_H_

#include <map>
#include <memory>
#include <utility>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/types.h>
#include <cutio-event/net/callbacks.h>

namespace cutio {
namespace event {

class Acceptor;
class EventLoop;
class InetAddress;
class ThreadModel;

/**
 * TCP server, supports single-threaded and thread-pool models.
 *
 * THis is an interface class, so don't expose too much details.
 */
class TcpServer : noncopyable {
 public:
  TcpServer(EventLoop* loop, const InetAddress& listen_addr);
  ~TcpServer();

  /**
   * Set the number of threads for handling input.
   *
   * Always accepts new connection in loop's thread.
   * Must be called before @c Start()
   * @param num_threads
   *    - 0 means all I/O in loop's thread, no thread will created. This is the default value.
   *    - 1 means all I/O in another thread.
   *    - N means a thread pool with N threads, new connections are assigned on a round-robin basis.
   */
   void SetThreadNum(int num_threads);

  /**
   * Starts the server if it's not listening.
   *
   * It's harmless to call it multiple times.
   * Not thread safe.
   */
  void Start();

  /**
   * Set connection callback.
   * Not thread safe.
   */
  void SetConnectionCallback(ConnectionCallback cb) { connection_cb_ = std::move(cb); }
  /**
   * Set message callback.
   * Not thread safe.
   */
  void SetReadCallback(MessageCallback cb) { message_cb_ = std::move(cb); }

 private:
  // Not thread safe.
  void NewConnection(int sockfd, const InetAddress& peer_addr);
  void RemoveConnection(const TcpConnectionPtr& conn);

  typedef std::map<string, TcpConnectionPtr> ConnectionMap;

 private:
  // The acceptor loop.
  EventLoop* loop_;
  std::unique_ptr<Acceptor> acceptor_;
  std::unique_ptr<ThreadModel> thread_model_;
  ConnectionCallback connection_cb_;
  MessageCallback message_cb_;
  const string server_name_;
  bool started_;
  // Always in loop thread.
  int next_conn_id_;
  ConnectionMap connections_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TCP_SERVER_H_
