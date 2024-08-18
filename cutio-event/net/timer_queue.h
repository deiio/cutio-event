/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_TIMER_QUEUE_H_
#define CUTIO_EVENT_NET_TIMER_QUEUE_H_

#include <list>

#include <cutio-event/base/mutex.h>
#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/timestamp.h>
#include <cutio-event/net/callbacks.h>
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
  // Called when timer_fd_ arms.
  void Timeout();
  // Insert timer in sorted list.
  bool InsertWithLockHold(Timer* timer);

 private:
  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  typedef std::list<Timer*> TimerList;

  EventLoop* loop_;
  const int timer_fd_;
  Channel timer_fd_channel_;
  MutexLock mutex_;
  // Timer list sorted by expiration, @guardedBy mutex_
  TimerList timers_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TIMER_QUEUE_H_