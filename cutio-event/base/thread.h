/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

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

  explicit Thread(ThreadFunc func);
  ~Thread();

  void Start();
  void Join();

  bool Started() const { return started_; }
  pthread_t PthreadId() const { return pthread_id_; }
  pid_t Tid() const { return tid_; }

 private:
  static void* StartThread(void* thread);

 private:
  bool started_;
  pthread_t pthread_id_;
  pid_t tid_;
  ThreadFunc func_;
};

namespace CurrentThread {

pid_t tid();

}  // namespace CurrentThread

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_THREAD_H_
