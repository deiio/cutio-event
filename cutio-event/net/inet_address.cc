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
const in_addr_t kInaddrAny = 0;  // INADDR_ANY

}  // anonymous namespace

InetAddress::InetAddress(uint16_t port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = sockets::HostToNetwork16(port);
  addr_.sin_addr.s_addr = sockets::HostToNetwork32(kInaddrAny);
}

InetAddress::InetAddress(string host, uint16_t port) {

}

}  // namespace event
}  // namespace cutio
