// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/utc_time.h>

#include <stdio.h>

using namespace cutio::event;

void passByConstReference(const UtcTime& x) {
  printf("%s\n", x.ToString().c_str());
}

void passByValue(UtcTime x) {
  printf("%s\n", x.ToString().c_str());
}

int main() {
  auto now(UtcTime::Now());
  printf("%s\n", now.ToString().c_str());
  passByConstReference(now);
  passByValue(now);
  return 0;
}