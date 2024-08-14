// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/inet_address.h>

#include <string.h>

#include <cutio-event/net/sockets_ops.h>

namespace cutio {
namespace event {
namespace {

typedef sockaddr SA;

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
const in_addr_t kInaddrAny = INADDR_ANY;  // INADDR_ANY
#pragma GCC diagnostic error "-Wold-style-cast"

}  // anonymous namespace

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in), "class InetAddress's size unexpected");

InetAddress::InetAddress(uint16_t port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = sockets::HostToNetwork16(port);
  addr_.sin_addr.s_addr = sockets::HostToNetwork32(kInaddrAny);
}

InetAddress::InetAddress(string host, uint16_t port) {
  (void)host;
  (void)port;
}

}  // namespace event
}  // namespace cutio
