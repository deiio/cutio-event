/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/sockets_ops.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cutio-event/base/logger.h>

namespace cutio {
namespace event {
namespace sockets {

typedef struct sockaddr SA;

int CreateNonblockingOrDie() {
  // Socket
  int sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
    LOG_SYSFATAL << "socket() failed";
  }

  // Non-blocking.
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  // FIXME: check return value
  int ret = ::fcntl(sockfd, F_SETFL, flags);
  if (ret < 0) {
    LOG_SYSFATAL << "set non-blocking failed";
  }

  // Close-on-exec.
  flags = ::fcntl(sockfd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  // FIXME: check return value
  ret = ::fcntl(sockfd, F_SETFD, flags);
  if (ret < 0) {
    LOG_SYSFATAL << "set close-on-exec failed";
  }

  return sockfd;
}

void BindOrDie(int sockfd, const sockaddr_in& addr) {
  int ret = ::bind(sockfd, reinterpret_cast<const SA*>(&addr), sizeof(addr));
  if (ret < 0) {
    LOG_SYSFATAL << "bind failed";
  }
}

void ListenOrDie(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) {
    LOG_SYSFATAL << "listen failed";
  }
}

int Accept(int sockfd, struct sockaddr_in* addr) {
  socklen_t addr_len = sizeof(*addr);
  int conn_fd = ::accept4(sockfd, reinterpret_cast<SA*>(addr), &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd == -1) {
    int saved_errno = errno;
    LOG_DEBUG << "accept: " << strerror(saved_errno);
    switch (saved_errno) {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO:
      case EPERM:
        // Expected errors.
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case EMFILE:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        // Unexpected errors.
        LOG_ERROR << "Accept() " << strerror(saved_errno);
        break;
      default:
        // Unknown errors.
        LOG_ERROR << "Accept() unexpected error " << strerror(saved_errno);
        break;
    }
  }

  return conn_fd;
}

void Close(int sockfd) {
  ::close(sockfd);
}

void Shutdown(int sockfd) {
  ::shutdown(sockfd, SHUT_RDWR);
}

void ToHostPort(char* buf, size_t size, const sockaddr_in& addr) {
  char host[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
  uint16_t port = NetworkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}

sockaddr_in GetLocalAddr(int sockfd) {
  sockaddr_in local_addr{};
  socklen_t addr_len = sizeof(local_addr);
  int ret = ::getsockname(sockfd, reinterpret_cast<SA*>(&local_addr), &addr_len);
  if (ret < 0) {
    LOG_SYSERR << "getsockname(" << sockfd << ") failed";
  }
  return local_addr;
}

}  // namespace sockets
}  // namespace event
}  // namespace cutio
