// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_INET_ADDRESS_H_
#define CUTIO_EVENT_NET_INET_ADDRESS_H_

#include <netinet/in.h>

#include <cutio-event/base/types.h>

namespace cutio {
namespace event {

/**
 * Wrapper of sockaddr_in.
 */
class InetAddress {
 public:
  /**
   * Constructs an endpoint with given port number.
   * Mostly used in TcpServer.
   */
  explicit InetAddress(uint16_t port);

  /**
   * Constructs an endpoint with given host and port.
   * @c host could either be "1.2.3.4" or "furzoom.com"
   */
   InetAddress(string host, uint16_t port);

 private:
  sockaddr_in addr_{};
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_INET_ADDRESS_H_
