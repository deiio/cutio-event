/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/timer_queue.h>

#include <cassert>
#include <cstring>
#include <sys/timerfd.h>
#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/net/channel.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/timer.h>
#include <cutio-event/net/timer_id.h>

namespace cutio {
namespace event {
namespace {

int CreateTimerFd() {
  int timer_fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (timer_fd < 0) {
    LOG_SYSFATAL << "Failed in timerfd_create()";
  }
  return timer_fd;
}

struct timespec HowMuchTimeFromNow(Timestamp when) {
  int64_t microseconds = when.MicroSecondsSinceEpoch() - Timestamp::Now().MicroSecondsSinceEpoch();
  if (microseconds < 100) {
    microseconds = 100;
  }

  timespec ts{};
  ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

void ResetTimerFd(int timer_fd, Timestamp when) {
  // Wake up loop by timerfd_settime().
  itimerspec new_value{};
  itimerspec old_value{};
  memset(&new_value, 0, sizeof(new_value));
  memset(&old_value, 0, sizeof(old_value));
  new_value.it_value = HowMuchTimeFromNow(when);
  int ret = timerfd_settime(timer_fd, 0, &new_value, &old_value);
  if (ret) {
    LOG_SYSERR << "Error in timerfd_settime()";
  }
}

}  // anonymous namespace

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timer_fd_(CreateTimerFd()),
    timer_fd_channel_(loop, timer_fd_),
    timers_() {
  timer_fd_channel_.SetReadCallback([this] { Timeout(); });
  // We are always reading the timer_fd_, we disarm it with timefd_settime.
  timer_fd_channel_.SetEvents(Channel::kReadEvent);
  loop_->UpdateChannel(&timer_fd_channel_);
}

TimerQueue::~TimerQueue() {
  ::close(timer_fd_);
  // Don't remove the channel, since we're in EventLoop::dtor();
  for (auto* timer : timers_) {
    delete timer;
  }
}

TimerId TimerQueue::Schedule(const TimerCallback& cb, Timestamp when, double interval) {
  auto* timer = new Timer(cb, when, interval);
  bool earliest_changed;
  {
    MutexLockGuard lock(mutex_);
    earliest_changed = InsertWithLockHold(timer);
  }

  if (earliest_changed) {
    ResetTimerFd(timer_fd_, when);
  }

  return TimerId(timer);
}

void TimerQueue::Cancel(TimerId timer_id) {

}

// FIXME: replace linked-list operations with binary-heap.
void TimerQueue::Timeout() {
  loop_->AssertInLoopThread();
  Timestamp now = Timestamp::Now();
  uint64_t how_many;
  ssize_t n = ::read(timer_fd_, &how_many, sizeof(how_many));
  LOG_DEBUG << "TimerQueue::timeout() " << how_many << " at " << now.ToString();
  if (n != sizeof(how_many)) {
    LOG_ERROR << "TimerQueue::timeout() reads " << n << " bytes instead of 8";
  }

  TimerList expired;
  // Move out all expired timers.
  {
    MutexLockGuard lock(mutex_);
    // Shall never call back in critical section.
    auto it = timers_.begin();
    while (it != timers_.end() && !(*it)->Expiration().After(now)) {
      ++it;
    }
    assert(it == timers_.end() || (*it)->Expiration().After(now));
    expired.splice(expired.begin(), timers_, timers_.begin(), it);
  }

  // Safe to callback outside critical section.
  for (auto& timer : expired) {
    timer->Run();
  }

  Timestamp next_expire;
  {
    MutexLockGuard lock(mutex_);
    //Shall never call back in critical section
    for (auto& timer : expired) {
      if (timer->Repeat()) {
        timer->Restart(now);
        InsertWithLockHold(timer);
      } else {
        // FIXME: move to a free list.
        delete timer;
      }
    }
    if (!timers_.empty()) {
      next_expire = timers_.front()->Expiration();
    }
  }

  if (next_expire.Valid()) {
    ResetTimerFd(timer_fd_, next_expire);
  }
}

bool TimerQueue::InsertWithLockHold(Timer* timer) {
  bool earliest_changed = false;
  Timestamp when = timer->Expiration();
  auto it = timers_.begin();
  if (it == timers_.end() || (*it)->Expiration().After(when)) {
    timers_.push_front(timer);
    earliest_changed = true;
  } else {
    while (it != timers_.end() && (*it)->Expiration().Before(when)) {
      ++it;
    }
    timers_.insert(it, timer);
  }

  return earliest_changed;
}

}  // namespace event
}  // namespace cutio
