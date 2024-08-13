// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_SOCKETS_OPS_H_
#define CUTIO_EVENT_NET_SOCKETS_OPS_H_

#include <arpa/inet.h>

namespace cutio {
namespace event {
namespace sockets {

inline uint32_t HostToNetwork32(uint32_t host_long) {
  return htonl(host_long);
}

inline uint16_t HostToNetwork16(uint16_t host_short) {
  return htons(host_short);
}

inline uint32_t NetworkToHost32(uint32_t net_long) {
  return ntohl(net_long);
}

inline uint16_t NetworkToHost16(uint16_t net_short) {
  return ntohs(net_short);
}

/**
 * Creates a non-blocking socket file descriptor, abort if any error.
 */
int CreateNonblockingOrDie();

}  // namespace sockets
}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_SOCKETS_OPS_H_