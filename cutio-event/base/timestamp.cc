// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/timestamp.h>

#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>

namespace cutio {
namespace event {

Timestamp::Timestamp()
  : microSecondsSinceEpoch_(0){}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
  : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

string Timestamp::ToString() const {
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64,
           seconds, microseconds);
  return buf;
}

Timestamp Timestamp::Now() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::Invalid() {
  return {};
}

} // event
} // cutio