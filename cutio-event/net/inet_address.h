/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_INET_ADDRESS_H_
#define CUTIO_EVENT_NET_INET_ADDRESS_H_

#include <netinet/in.h>

#include <cutio-event/base/copyable.h>
#include <cutio-event/base/types.h>

namespace cutio {
namespace event {

/**
 * Wrapper of sockaddr_in.
 *
 * This is an interface class.
 */
class InetAddress : public copyable {
 public:
  /**
   * Constructs an endpoint with given port number.
   * Mostly used in TcpServer listening.
   */
  explicit InetAddress(uint16_t port);

  /**
   * Constructs an endpoint with given host and port.
   * @c host could either be "1.2.3.4" or "furzoom.com"
   */
  InetAddress(string host, uint16_t port);

  /**
   * Constructs an endpoint with given struct @c addr.
   * Mostly used when accepting new connections.
   */
  explicit InetAddress(const sockaddr_in& addr) : addr_(addr) {}

   // Default copy/assignment are Okay.

  string ToHostPort() const;

  const sockaddr_in& GetSockAddrInet() const { return addr_; }
  void SetSockAddrInet(const sockaddr_in& addr) { addr_ = addr; }

 private:
  sockaddr_in addr_{};
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_INET_ADDRESS_H_
