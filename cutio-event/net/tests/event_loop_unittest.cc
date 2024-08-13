// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <stdio.h>
#include <unistd.h>

#include <functional>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>

using namespace cutio::event;

void ThreadFunc() {
  EventLoop loop;
  loop.Loop();
}

int main() {
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

  EventLoop loop;

  Thread thread(ThreadFunc);
  thread.Start();

  loop.Loop();

  return 0;
}
