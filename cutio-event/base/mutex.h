// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_BASE_MUTEX_H_
#define CUTIO_EVENT_BASE_MUTEX_H_

#include <pthread.h>

#include <cutio-event/base/noncopyable.h>

namespace cutio {
namespace event {

class MutexLock : noncopyable {
 public:
  MutexLock() {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock() {
    pthread_mutex_unlock(&mutex_);
  }

  void Lock() {
    pthread_mutex_lock(&mutex_);
  }

  void UnLock() {
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* GetPthreadMutex() {
    return &mutex_;
  }

 private:
  pthread_mutex_t mutex_;
};

class MutexLockGuard : noncopyable {
 public:
  explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) {}

  ~MutexLockGuard() { mutex_.UnLock(); }

 private:
  MutexLock& mutex_;
};

}  // namespace event
}  // namespace cutio

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A temporary object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif  // CUTIO_EVENT_BASE_MUTEX_H_
