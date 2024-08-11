// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_BASE_THREAD_H_
#define CUTIO_EVENT_BASE_THREAD_H_

#include <functional>

#include <pthread.h>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class Thread : noncopyable {
 public:
  typedef std::function<void()> ThreadFunc;

  explicit Thread(ThreadFunc  func);
  ~Thread();

  void Start();
  void Join();

 private:
  pthread_t ptid_;
  ThreadFunc func_;
};

namespace CurrentThread {

pid_t tid();

}  // namespace CurrentThread

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_THREAD_H_
