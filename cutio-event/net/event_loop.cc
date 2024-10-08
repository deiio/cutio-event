/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/event_loop.h>

#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/mutex.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/net/channel.h>
#include <cutio-event/net/poller.h>
#include <cutio-event/net/timer_queue.h>

namespace cutio {
namespace event {
namespace {

__thread EventLoop* t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

int CreateEventFd() {
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0) {
    perror("Failed in eventfd");
    abort();
  }
  return evtfd;
}

}  // anonymous namespace

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    thread_id_(CurrentThread::tid()),
    poller_(Poller::NewDefaultPoller()),
    timer_queue_(new TimerQueue(this)),
    wakeup_fd_(CreateEventFd()),
    wakeup_channel_(new Channel(this, wakeup_fd_)) {
  LOG_TRACE << "EventLoop created in thread " << thread_id_;
  if (t_loopInThisThread) {
    LOG_FATAL << "another EventLoop " << t_loopInThisThread << " exists in this thread "
              << thread_id_;
  } else {
    t_loopInThisThread = this;
  }

  wakeup_channel_->SetReadCallback([this] { WakedUp(); });
  // We are always reading the wakeup_fd_, like the old(2) way.
  wakeup_channel_->SetEvents(Channel::kReadEvent);
  UpdateChannel(wakeup_channel_.get());
}

EventLoop::~EventLoop() {
  ::close(wakeup_fd_);
  t_loopInThisThread = nullptr;
}

void EventLoop::Loop() {
  assert(!looping_);
  AssertInLoopThread();
  looping_ = true;
  while (!quit_) {
    active_channels_.clear();
    poller_->Poll(kPollTimeMs, &active_channels_);
    for (auto& channel : active_channels_) {
      channel->HandleEvent();
    }
  }
  looping_ = false;
}

void EventLoop::Quit() {
  quit_ = true;
}

void EventLoop::Wakeup() {
  uint64_t one = 1;
  ssize_t n = ::write(wakeup_fd_, &one, sizeof(one));
  if (n != sizeof(one)) {
    LOG_ERROR << "EventLoop::wakeup() write " << n << " bytes instead of 8";
  }
}

void EventLoop::RunInLoop(const Functor& cb) {
  if (thread_id_ == CurrentThread::tid()) {
    cb();
  } else {
    abort();
  }
}

void EventLoop::RunDelayDestruct(const EventLoop::Functor& cb) {

}

TimerId EventLoop::RunAt(const Timestamp& time, const TimerCallback& cb) {
  return timer_queue_->Schedule(cb, time, 0.0);
}

TimerId EventLoop::RunAfter(double delay, const TimerCallback& cb) {
  Timestamp time(AddTime(Timestamp::Now(), delay));
  return RunAt(time, cb);
}

TimerId EventLoop::RunEvery(double interval, const TimerCallback& cb) {
  Timestamp time(AddTime(Timestamp::Now(), interval));
  return timer_queue_->Schedule(cb, time, interval);
}

void EventLoop::Cancel(TimerId timer_id) {

}

void EventLoop::UpdateChannel(Channel* channel) {
  assert(channel->GetLoop() == this);
  AssertInLoopThread();
  poller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel* channel) {
  assert(channel->GetLoop() == this);
  poller_->RemoveChannel(channel);
}

void EventLoop::AssertInLoopThread() const {
  assert(thread_id_ == CurrentThread::tid());
}

void EventLoop::WakedUp() {

}

}  // namespace event
}  // namespace cutio
