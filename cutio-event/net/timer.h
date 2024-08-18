/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_TIMER_H_
#define CUTIO_EVENT_NET_TIMER_H_

#include <functional>
#include <utility>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/timestamp.h>
#include <cutio-event/net/callbacks.h>

namespace cutio {
namespace event {

/**
 * Internal class for timer event.
 */
class Timer : noncopyable {
 public:
  Timer(TimerCallback cb, Timestamp when, double interval)
    : cb_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0) {}

  void Run() const { cb_(); }

  Timestamp Expiration() const { return expiration_; }
  bool Repeat() const { return repeat_; }

  void Restart(Timestamp now);

 private:
  const TimerCallback cb_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TIMER_H_