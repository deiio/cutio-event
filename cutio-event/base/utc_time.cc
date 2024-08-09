// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/utc_time.h>

#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>

namespace cutio {
namespace event {

UtcTime::UtcTime()
  : microSecondsSinceEpoch_(0){}

UtcTime::UtcTime(int64_t microSecondsSinceEpoch)
  : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

string UtcTime::toString() const {
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64,
           seconds, microseconds);
  return buf;
}

UtcTime UtcTime::now() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int64_t seconds = tv.tv_sec;
  return UtcTime(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

} // event
} // cutio