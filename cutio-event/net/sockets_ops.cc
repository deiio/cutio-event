// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/sockets_ops.h>

#include <fcntl.h>
#include <sys/socket.h>

namespace cutio {
namespace event {
namespace sockets {

int CreateNonblockingOrDie() {
  // Socket
  int sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Reuse Address.
  int one = 1;
  ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

  // Non-blocking.
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  // FIXME: check return value
  ::fcntl(sockfd, F_SETFL, flags);

  // Close-on-exec.
  flags = ::fcntl(sockfd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  // FIXME: check return value
  ::fcntl(sockfd, F_SETFD, flags);

  return sockfd;
}

}  // namespace sockets
}  // namespace event
}  // namespace cutio
