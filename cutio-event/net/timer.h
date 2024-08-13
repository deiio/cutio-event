// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_TIMER_H_
#define CUTIO_EVENT_NET_TIMER_H_

#include <functional>
#include <utility>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/timestamp.h>

namespace cutio {
namespace event {

/**
 * Internal class for timer event.
 */
class Timer : noncopyable {
 public:
  typedef std::function<void()> TimerCallback;

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