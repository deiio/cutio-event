// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_TIMER_QUEUE_H_
#define CUTIO_EVENT_NET_TIMER_QUEUE_H_

#include <functional>
#include <list>

#include <cutio-event/base/mutex.h>
#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/timestamp.h>
#include <cutio-event/net/channel.h>

namespace cutio {
namespace event {

class EventLoop;
class Timer;
class TimerId;

/**
 * A best efforts timer queue.
 *
 * No guarantee that the callback will be on time.
 */
class TimerQueue : noncopyable {
 public:
  typedef std::function<void()> TimerCallback;

  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();

  /**
   * Schedules the callback to be run when given time,
   * repeats if @c interval > 0.
   *
   * Must be thread safe. Usually be called from other threads.
   */
  TimerId Schedule(const TimerCallback& cb, Timestamp when, double interval);

  void Cancel(TimerId timer_id);

 private:
  void Timeout();  // Called when timer_fd_ arms.
  bool InsertWithLockHold(Timer* timer);  // Insert timer in sorted list.

 private:
  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  typedef std::list<Timer*> TimerList;

  EventLoop* loop_;
  const int timer_fd_;
  Channel timer_fd_channel_;
  MutexLock mutex_;
  TimerList timers_;  // Timer list sorted by expiration, @guardedBy mutex_
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TIMER_QUEUE_H_