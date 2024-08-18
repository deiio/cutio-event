/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_BASE_CONDITION_H_
#define CUTIO_EVENT_BASE_CONDITION_H_

#include <cutio-event/base/mutex.h>
#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class Condition : noncopyable {
 public:
  explicit Condition(MutexLock& mutex)
    : mutex_(mutex) {
    pthread_cond_init(&cond_, nullptr);
  }

  ~Condition() {
    pthread_cond_destroy(&cond_);
  }

  void Wait() {
    pthread_cond_wait(&cond_, mutex_.GetPthreadMutex());
  }

  void Notify() {
    pthread_cond_signal(&cond_);
  }

  void NotifyAll() {
    pthread_cond_broadcast(&cond_);
  }

 private:
  MutexLock& mutex_;
  pthread_cond_t cond_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_CONDITION_H_
