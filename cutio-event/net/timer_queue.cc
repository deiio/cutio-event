// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/timer_queue.h>

#include <inttypes.h>
#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>

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
    perror("Failed in timerfd_create");
    abort();
  }
  return timer_fd;
}

struct timespec HowMuchTimeFromNow(UtcTime then) {
  int64_t microseconds = then.MicroSecondsSinceEpoch() - UtcTime::Now().MicroSecondsSinceEpoch();
  if (microseconds < 100) {
    microseconds = 100;
  }

  timespec ts;
  ts.tv_sec = static_cast<time_t>(microseconds / UtcTime::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>((microseconds % UtcTime::kMicroSecondsPerSecond) * 1000);
  return ts;
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
}

TimerId TimerQueue::Schedule(const TimerQueue::TimerCallback& cb, UtcTime at, double interval) {
  Timer* timer = new Timer(cb, at, interval);
  bool earliest_changed = false;
  {
    MutexLockGuard lock(mutex_);
    auto it = timers_.begin();
    if (it == timers_.end() || (*it)->Expiration().After(at)) {
      timers_.push_front(timer);
      earliest_changed = true;
    } else {
      while (it != timers_.end() && (*it)->Expiration().Before(at)) {
        ++it;
      }
      timers_.insert(it, timer);
    }
  }

  if (earliest_changed) {
    // Wake up loop by timerfd_settime().
    itimerspec new_value;
    itimerspec old_value;
    memset(&new_value, 0, sizeof(new_value));
    memset(&old_value, 0, sizeof(old_value));
    new_value.it_value = HowMuchTimeFromNow(at);
    int ret = timerfd_settime(timer_fd_, 0, &new_value, &old_value);
    if (ret) {
      perror("Error in timerfd_settime");
    }
  }

  return TimerId(timer);
}

void TimerQueue::Cancel(TimerId timer_id) {

}

void TimerQueue::Timeout() {
  uint64_t how_many;
  ssize_t n = ::read(timer_fd_, &how_many, sizeof(how_many));
  printf("timeout %" PRIu64 "\n", how_many);
  if (n != sizeof(how_many)) {
    fprintf(stderr, "TimerQueue::timeout() reads %zd bytes instead of 8\n", n);
  }
}

}  // namespace event
}  // namespace cutio
