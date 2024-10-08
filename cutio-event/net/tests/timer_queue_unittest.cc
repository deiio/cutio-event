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

int cnt = 0;
EventLoop* g_loop;

void print(const char* msg) {
  LOG_INFO << "msg " << Timestamp::Now().ToString() << " " << msg;
  if (++cnt == 20) {
    g_loop->Quit();
  }
}

int main() {
  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  sleep(1);

  {
    EventLoop loop;
    g_loop = &loop;
    print("main");
    loop.RunAfter(1, std::bind(print, "once1"));
    loop.RunAfter(1.5, std::bind(print, "once1.5"));
    loop.RunAfter(2.5, std::bind(print, "once2.5"));
    loop.RunAfter(3.5, std::bind(print, "once3.5"));
    loop.RunEvery(2, std::bind(print, "every2"));
    loop.RunEvery(3, std::bind(print, "every3"));

    loop.Loop();
    print("exit");
  }

  return 0;
}
