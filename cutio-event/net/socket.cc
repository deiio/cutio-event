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

#include <cutio-event/net/socket.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>

#include <sys/socket.h>
#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/net/inet_address.h>
#include <cutio-event/net/sockets_ops.h>

namespace cutio {
namespace event {

Socket::~Socket() {
  sockets::Close(sockfd_);
}

void Socket::BindAddress(const InetAddress& local_addr) {
  sockets::BindOrDie(sockfd_, local_addr.GetSockAddrInet());
}

void Socket::Listen() {
  sockets::ListenOrDie(sockfd_);
}

int Socket::Accept(InetAddress* peer_addr) {
  sockaddr_in addr{};
  int conn_fd = sockets::Accept(sockfd_, &addr);
  if (conn_fd >= 0) {
    *peer_addr->GetMutableSockAddrInet() = addr;
  }
  return conn_fd;
}

void Socket::Shutdown() {
  // shutdown
}

void Socket::SetTcpNoDelay(bool on) {
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
  if (ret < 0) {
    LOG_SYSERR << "setsocopt TCP_NODELAY failed";
  }
}

void Socket::SetReuseAddr(bool on) {
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  if (ret < 0) {
    LOG_SYSERR << "setsocopt SO_REUSEADDR failed";
  }
}

}  // namespace event
}  // namespace cutio
