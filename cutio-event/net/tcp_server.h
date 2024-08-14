// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_TCP_SERVER_H_
#define CUTIO_EVENT_NET_TCP_SERVER_H_

#include <memory>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class Acceptor;
class EventLoop;
class InetAddress;

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
   * Starts the server if it's not listening.
   *
   * It's harmless to call it multiple times.
   */
  void Start();

 private:
  EventLoop* loop_;
  std::unique_ptr<Acceptor> acceptor_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TCP_SERVER_H_
