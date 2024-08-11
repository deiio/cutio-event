// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <stdio.h>
#include <unistd.h>

#include <cutio-event/net/event_loop.h>

using namespace cutio::event;

void print() {
  printf("%s\n", UtcTime::Now().ToString().c_str());
}

int main() {
  printf("pid = %d\n", getpid());
  EventLoop loop;

  print();
  loop.RunAfter(1.0, print);

  loop.Loop();

  return 0;
}
