/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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

void ToHostPort(char* buf, size_t size, const struct sockaddr_in& addr);
sockaddr_in GetLocalAddr(int sockfd);

}  // namespace sockets
}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_SOCKETS_OPS_H_
