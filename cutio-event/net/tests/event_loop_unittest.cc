// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <stdio.h>
#include <unistd.h>

#include <functional>

#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>

using namespace cutio::event;

void ThreadFunc() {
  EventLoop loop;
  loop.Loop();
}

int main() {
  printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

  EventLoop loop;

  Thread thread(ThreadFunc);
  thread.Start();

  loop.Loop();

  return 0;
}
