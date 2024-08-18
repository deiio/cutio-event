/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_CHANNEL_H_
#define CUTIO_EVENT_NET_CHANNEL_H_

#include <functional>
#include <memory>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class EventLoop;

/**
 * A selectable I/O channel.
 *
 * This class doesn't own the file descriptor.
 * The file descriptor could be a socket, an eventfd, a timerfd,
 * or a singalfd.
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
  void SetCloseCallback(const EventCallback& cb) { close_callback_ = cb; }
  void SetErrorCallback(const EventCallback& cb) { error_callback_ = cb; }

  /**
   * Tie this channel to the owner object managed by shared_ptr,
   * prevent the owner object being destroyed in HandleEvent.
   */
  void Tie(const std::shared_ptr<void>&);

  int Fd() const { return fd_; }
  int Events() const { return events_; }

  void SetEvents(int evt) { events_ = evt; }
  void SetREvents(int revt) { revents_ = revt; }

  int Index() const { return index_; }
  void SetIndex(int idx) { index_ = idx; }

  EventLoop* GetLoop() { return loop_; }

 private:
  void HandleEventWithGuard();

 private:
  EventLoop*    loop_;
  const int     fd_;
  int           events_;
  int           revents_;
  int           index_;   // Used by PollPoller

  std::weak_ptr<void> tie_;
  bool                tied_;

  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_CHANNEL_H_
