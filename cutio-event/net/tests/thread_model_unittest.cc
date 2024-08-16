// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

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