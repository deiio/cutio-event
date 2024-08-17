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

#include <assert.h>
#include <unistd.h>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/thread_model.h>

using namespace cutio::event;

int main() {
  LOG_DEBUG << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

  EventLoop loop;
  loop.RunAfter(11, [&loop] { loop.Quit(); });

  {
    LOG_DEBUG << "single thread";
    ThreadModel model(&loop);
    model.SetThreadNum(0);
    model.Start();
    assert(&loop == model.GetNextLoop());
    assert(&loop == model.GetNextLoop());
    assert(&loop == model.GetNextLoop());
  }

  {
    LOG_DEBUG << "Another thread";
    ThreadModel model(&loop);
    model.SetThreadNum(1);
    model.Start();
    auto* next_loop = model.GetNextLoop();
    assert(next_loop != &loop);
    assert(next_loop == model.GetNextLoop());
    assert(next_loop == model.GetNextLoop());
  }

  {
    LOG_DEBUG << "Three thread";
    ThreadModel model(&loop);
    model.SetThreadNum(3);
    model.Start();
    auto* next_loop = model.GetNextLoop();
    assert(next_loop != &loop);
    assert(next_loop != model.GetNextLoop());
    assert(next_loop != model.GetNextLoop());
    assert(next_loop == model.GetNextLoop());
  }

  loop.Loop();
}