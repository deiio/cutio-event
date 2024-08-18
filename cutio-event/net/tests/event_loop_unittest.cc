/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <stdio.h>
#include <unistd.h>

#include <functional>

#include <cutio-event/base/logger.h>
#include <cutio-event/base/thread.h>
#include <cutio-event/net/event_loop.h>

using namespace cutio::event;

void Callback() {
  LOG_DEBUG << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop anotherLoop;
}

void ThreadFunc() {
  LOG_DEBUG << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop loop;
  loop.RunAfter(1.0, Callback);
  loop.Loop();
}

int main() {
  LOG_DEBUG << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

  EventLoop loop;

  Thread thread(ThreadFunc);
  thread.Start();

  loop.Loop();

  return 0;
}
