/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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