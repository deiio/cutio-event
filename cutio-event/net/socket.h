/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_SOCKET_H_
#define CUTIO_EVENT_NET_SOCKET_H_

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class InetAddress;

/*
 * Wrapper of socket file descriptor.
 *
 * It closes the sockfd when destructs.
 *
 * It's thread safe, all operations are delegated to OS.
 */
class Socket : noncopyable {
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd) {}

  ~Socket();

  int Fd() const { return sockfd_; }

  /*
   * Abort if address in use.
   */
  void BindAddress(const InetAddress& local_addr);

  /**
   * listen
   */
  void Listen();

  /**
   * On success, returns a non-negative integer that is a descriptor for
   * the accepted socket, which has been set to non-blocking and close-on-exec.
   * *peer_addr is assigned. On error, -1 is returned, and *peer_addr is
   * untouched.
   */
  int Accept(InetAddress* peer_addr);

  void Shutdown();

  /**
   * Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
   */
  void SetTcpNoDelay(bool on);

  /**
   * Enable/disable SO_REUSEADDR.
   */
  void SetReuseAddr(bool on);

 private:
  const int sockfd_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_SOCKET_H_
