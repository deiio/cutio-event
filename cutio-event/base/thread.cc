// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/thread.h>

#include <assert.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

namespace cutio {
namespace event {

namespace {

__thread pid_t t_cached_tid = 0;

pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

}  // anonymous namespace

Thread::Thread(Thread::ThreadFunc func)
  : started_(false),
    pthread_id_(0),
    tid_(0),
    func_(std::move(func)) {}

Thread::~Thread() {}

void Thread::Start() {
  assert(!started_);
  started_ = true;
  pthread_create(&pthread_id_, nullptr, &StartThread, this);
}

void Thread::Join() {
  assert(started_);
  pthread_join(pthread_id_, nullptr);
}

void* Thread::StartThread(void* thread) {
  auto* t = static_cast<Thread*>(thread);
  t->tid_ = CurrentThread::tid();
  t->func_();
  return nullptr;
}

pid_t CurrentThread::tid() {
  if (t_cached_tid == 0) {
    t_cached_tid = gettid();
  }
  return t_cached_tid;
}

}  // namespace event
}  // namespace cutio
