/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_CALLBACKS_H_
#define CUTIO_EVENT_NET_CALLBACKS_H_

#include <functional>
#include <memory>

namespace cutio {
namespace event {

class ChannelBuffer;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
// The data has been read to (buf, len)
typedef std::function<void(const TcpConnectionPtr&, ChannelBuffer*)> MessageCallback;

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_CALLBACKS_H_
