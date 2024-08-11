// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/thread.h>

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <utility>

namespace cutio {
namespace event {

namespace {

__thread pid_t t_tid = 0;

pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void* StartThread(void *cb) {
  auto* func = static_cast<Thread::ThreadFunc*>(cb);
  t_tid = gettid();
  (*func)();
  return nullptr;
}

}  // anonymous namespace

Thread::Thread(Thread::ThreadFunc func)
  : ptid_(0),
    func_(std::move(func)) {}

Thread::~Thread() {}

void Thread::Start() {
  pthread_create(&ptid_, nullptr, &StartThread, &func_);
}

void Thread::Join() {
  pthread_join(ptid_, nullptr);
}

pid_t CurrentThread::tid() {
  if (t_tid == 0) {
    t_tid = gettid();
  }
  return t_tid;
}

}  // namespace event
}  // namespace cutio
