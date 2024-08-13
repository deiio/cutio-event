// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/timestamp.h>

#include <cutio-event/base/logger.h>

using namespace cutio::event;

void passByConstReference(const Timestamp& x) {
  LOG_TRACE << x.ToString();
}

void passByValue(Timestamp x) {
  LOG_TRACE << x.ToString();
}

int main() {
  auto now(Timestamp::Now());
  LOG_TRACE << now.ToString();
  passByConstReference(now);
  passByValue(now);
  return 0;
}