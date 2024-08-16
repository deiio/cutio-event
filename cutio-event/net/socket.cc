// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

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
