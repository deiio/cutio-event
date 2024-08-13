// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/timestamp.h>

#include <stdio.h>

using namespace cutio::event;

void passByConstReference(const Timestamp& x) {
  printf("%s\n", x.ToString().c_str());
}

void passByValue(Timestamp x) {
  printf("%s\n", x.ToString().c_str());
}

int main() {
  auto now(Timestamp::Timestamp());
  printf("%s\n", now.ToString().c_str());
  passByConstReference(now);
  passByValue(now);
  return 0;
}