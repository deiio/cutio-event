/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

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

void BindOrDie(int sockfd, const struct sockaddr_in& addr);
void ListenOrDie(int sockfd);
int  Accept(int sockfd, struct sockaddr_in* addr);
void Close(int sockfd);
void Shutdown(int sockfd);

void ToHostPort(char* buf, size_t size, const struct sockaddr_in& addr);
sockaddr_in GetLocalAddr(int sockfd);

}  // namespace sockets
}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_SOCKETS_OPS_H_
