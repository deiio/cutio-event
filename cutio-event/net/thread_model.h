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
