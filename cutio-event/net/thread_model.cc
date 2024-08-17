/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <cutio-event/net/thread_model.h>

#include <assert.h>

#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>

namespace cutio {
namespace event {

ThreadModel::ThreadModel(EventLoop* base_loop)
  : base_loop_(base_loop),
    started_(false),
    exiting_(false),
    num_threads_(0),
    next_(0),
    mutex_(),
    cond_(mutex_) {}

ThreadModel::~ThreadModel() {
  exiting_ = true;
  for (auto& loop : loop_poll_) {
    loop->Quit();
    // Don't delete loop, it's stack variable.
  }

  for (auto* t : threads_) {
    t->Join();
    delete t;
  }
}

void ThreadModel::Start() {
  assert(!started_);
  base_loop_->AssertInLoopThread();

  started_ = true;

  for (int i = 0; i < num_threads_; i++) {
    auto *t = new Thread([this] { ThreadFunc(); });
    t->Start();
    threads_.push_back(t);
  }

  {
    MutexLockGuard lock(mutex_);
    while (static_cast<int>(loop_poll_.size()) != num_threads_) {
      cond_.Wait();
    }
  }
}

EventLoop* ThreadModel::GetNextLoop() {
  base_loop_->AssertInLoopThread();
  EventLoop* loop = base_loop_;

  if (!loop_poll_.empty()) {
    // Round-robin
    loop = loop_poll_[next_];
    ++next_;
    if (next_ >= loop_poll_.size()) {
      next_ = 0;
    }
  }

  return loop;
}

void ThreadModel::ThreadFunc() {
  EventLoop loop;

  {
    MutexLockGuard lock(mutex_);
    loop_poll_.push_back(&loop);
    cond_.Notify();
  }

  loop.Loop();
  assert(exiting_);
}

}  // namespace event
}  // namespace cutio
