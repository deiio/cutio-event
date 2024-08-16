// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_TCP_SERVER_H_
#define CUTIO_EVENT_NET_TCP_SERVER_H_

#include <memory>
#include <utility>

#include <cutio-event/base/noncopyable.h>
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
  void NewConnection(int fd, const InetAddress& peer_addr);

 private:
  // The acceptor loop.
  EventLoop* loop_;
  std::unique_ptr<Acceptor> acceptor_;
  std::unique_ptr<ThreadModel> thread_model_;
  ConnectionCallback connection_cb_;
  MessageCallback message_cb_;
  bool started_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TCP_SERVER_H_
