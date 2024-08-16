// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_NET_THREAD_MODEL_H_
#define CUTIO_EVENT_NET_THREAD_MODEL_H_

#include <vector>

#include <cutio-event/base/condition.h>
#include <cutio-event/base/mutex.h>
#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class EventLoop;
class Thread;

class ThreadModel : noncopyable {
 public:
  ThreadModel(EventLoop* base_loop);
  ~ThreadModel();

  void SetThreadNum(int num_threads) { num_threads_ = num_threads; }
  void Start();
  EventLoop* GetNextLoop();

 private:
  void ThreadFunc();

 private:
  EventLoop* base_loop_;
  bool started_;
  bool exiting_;
  int num_threads_;
  size_t next_;
  std::vector<Thread*> threads_;
  MutexLock mutex_;
  Condition cond_;
  std::vector<EventLoop*> loop_poll_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_THREAD_MODEL_H_
