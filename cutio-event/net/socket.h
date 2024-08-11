// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_SOCKET_H_
#define CUTIO_EVENT_NET_SOCKET_H_

namespace cutio {
namespace event {

/*
 * Wrapper of socket file descriptor.
 *
 * It closes the sockfd when destructs.
 */
class Socket {
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd) {}

  ~Socket();

  int Fd() const { return sockfd_; }

  /**
   * Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
   */
  void SetTcpNoDelay(bool on);

 private:
  int sockfd_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_SOCKET_H_
