// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/utc_time.h>

#include <stdio.h>

void passByConstReference(const cutio::event::UtcTime& x) {
  printf("%s\n", x.toString().c_str());
}

void passByValue(cutio::event::UtcTime x) {
  printf("%s\n", x.toString().c_str());
}

int main() {
  auto now(cutio::event::UtcTime::now());
  printf("%s\n", now.toString().c_str());
  passByConstReference(now);
  passByValue(now);
  return 0;
}