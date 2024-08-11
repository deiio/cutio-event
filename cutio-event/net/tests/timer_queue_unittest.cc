// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/net/event_loop.h>
#include <cutio-event/net/timer_queue.h>

using namespace cutio::event;

int main() {
  EventLoop loop;
  TimerQueue queue(&loop);

  return 0;
}
