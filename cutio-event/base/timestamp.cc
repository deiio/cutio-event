/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/base/timestamp.h>

#include <inttypes.h>
#include <stdio.h>
#include <sys/time.h>

namespace cutio {
namespace event {

static_assert(sizeof(Timestamp) == sizeof(int64_t), "class Timestamp's size is not 8 bytes");

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

string Timestamp::ToFormattedString() const {
  char buf[32] = {0};
  auto seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
  auto microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
  tm tm_time{};
  gmtime_r(&seconds, &tm_time);

  snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%06d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
           microseconds);
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