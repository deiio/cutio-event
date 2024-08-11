// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_CHANNEL_H_
#define CUTIO_EVENT_NET_CHANNEL_H_

#include <functional>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class EventLoop;

/**
 * A selectable I/O channel.
 *
 * This class doesn't own the file descriptor.
 */
class Channel : noncopyable {
 public:
  typedef std::function<void()> EventCallback;
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
  static const int kErrorEvent;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void HandleEvent();
  void SetReadCallback(const EventCallback& cb) { read_callback_ = cb; }
  void SetWriteCallback(const EventCallback& cb) { write_callback_ = cb; }
  void SetErrorCallback(const EventCallback& cb) { error_callback_ = cb; }

  int Fd() const { return fd_; }
  int Events() const { return events_; }

  void SetEvents(int evt) { events_ = evt; }
  void SetREvents(int revt) { revents_ = revt; }

  EventLoop* GetLoop() { return loop_; }

 private:
  EventLoop*    loop_;
  const int     fd_;
  int           events_;
  int           revents_;
  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback error_callback_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_CHANNEL_H_
