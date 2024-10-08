/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_EVENT_LOOP_H_
#define CUTIO_EVENT_NET_EVENT_LOOP_H_

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/timestamp.h>
#include <cutio-event/net/callbacks.h>
#include <cutio-event/net/timer_id.h>

namespace cutio {
namespace event {

class Channel;
class Poller;
class TimerQueue;

/*
 * Reactor, at most one per thread.
 *
 * This is an interface class, so don't expose too much details.
 */
class EventLoop : noncopyable {
 public:
  typedef std::function<void()> Functor;

  EventLoop();
  ~EventLoop();

  /**
   * Loops forever.
   *
   * Must be called in the same thread as creation of the object.
   */
  void Loop();

  void Quit();
  void Wakeup();

  /**
   * Timers.
   * Runs callback immediately in the loop thread.
   * It wakes up the loop, and run the cb. If in the same loop thread,
   * cb is run within the function.
   * Safe to call from other threads.
   */
  void RunInLoop(const Functor& cb);

  /**
   * Queues callback in the loop thread.
   * Runs after finish pooling.
   * Safe to call from other threads.
   */
  void RunDelayDestruct(const Functor& cb);

  /**
   * Runs callback at the specified time @c time.
   * Safe to call from other threads.
   */
  TimerId RunAt(const Timestamp& time, const TimerCallback& cb);

  /**
   * Runs callback after @c delay seconds.
   * Safe to call from other threads.
   */
  TimerId RunAfter(double delay, const TimerCallback& cb);

  /**
   * Runs callback every @c interval seconds.
   * Safe to call from other threads.
   */
  TimerId RunEvery(double interval, const TimerCallback& cb);

  /**
   * Cancels the timer.
   * Safe to cal from other threads.
   */
  void Cancel(TimerId timer_id);

  void UpdateChannel(Channel* channel);
  void RemoveChannel(Channel* channel);

  pid_t ThreadId() const { return thread_id_; }
  void AssertInLoopThread() const;

 private:
  void WakedUp();

 private:
  typedef std::vector<Channel*> ChannelList;

  std::atomic_bool looping_;
  std::atomic_bool quit_;
  const pid_t thread_id_;
  std::unique_ptr<Poller> poller_;
  std::unique_ptr<TimerQueue> timer_queue_;
  int wakeup_fd_;
  // Unlink in TimerQueue, which is an internal calss,
  // we don't expose Channel to client.
  std::unique_ptr<Channel> wakeup_channel_;
  ChannelList active_channels_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_EVENT_LOOP_H_
