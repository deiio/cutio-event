// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_CALLBACKS_H_
#define CUTIO_EVENT_NET_CALLBACKS_H_

#include <functional>

namespace cutio {
namespace event {

class TcpConnection;

typedef std::function<void()> TimerCallback;
typedef std::function<void(TcpConnection*)> ConnectionCallback;
// The data has been read to (buf, len)
typedef std::function<void(TcpConnection*, const void* buf, ssize_t len)> MessageCallback;

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_CALLBACKS_H_
