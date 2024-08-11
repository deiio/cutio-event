// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_BASE_UTC_TIME_H_
#define CUTIO_EVENT_BASE_UTC_TIME_H_

#include <cutio-event/base/types.h>

namespace cutio {
namespace event {

/**
 * Time stamp in UTC, in microseconds resolution.
 *
 * This class is immutable.
 * It's recommended to pass it by value, since it's passed in register
 * on x64.
 */
class UtcTime {
 public:
  // Constructs an Invalid UtcTime.
  UtcTime();

  // Constructs a UtcTime at specific time.
  explicit UtcTime(int64_t microSecondsSinceEpoch);

  // Default copy/assignment are okay.

  string ToString() const;

  bool Valid() const {
    return microSecondsSinceEpoch_ > 0;
  }

  bool Before(UtcTime rhs) const {
    return microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
  }

  bool After(UtcTime rhs) const {
    return microSecondsSinceEpoch_ > rhs.microSecondsSinceEpoch_;
  }

  bool Equals(UtcTime rhs) const {
    return microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
  }

  // for internal usage.
  int64_t MicroSecondsSinceEpoch() const {
    return microSecondsSinceEpoch_;
  }

  // Get time of Now.
  static UtcTime Now();
  static UtcTime Invalid();
  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(UtcTime lhs, UtcTime rhs) {
  return lhs.Before(rhs);
}

inline bool operator==(UtcTime lhs, UtcTime rhs) {
  return lhs.Equals(rhs);
}

/**
 * Gets time difference of two timestamp, result in seconds.
 *
 * @param high, low
 * @return (high - low) in seconds
 */
inline double TimeDifference(UtcTime high, UtcTime low) {
  auto diff = high.MicroSecondsSinceEpoch() - low.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / UtcTime::kMicroSecondsPerSecond;
}

/**
 * Add @c seconds to given timestamp.
 *
 * @param timestamp, seconds
 * @return timestamp + seconds at UtcTime
 */
inline UtcTime AddTime(UtcTime timestamp, double seconds) {
  auto delta = static_cast<int64_t>(seconds * UtcTime::kMicroSecondsPerSecond);
  return UtcTime(timestamp.MicroSecondsSinceEpoch() + delta);
}

}  // namespace event
}  // namespace cutio

#endif // CUTIO_EVENT_BASE_UTC_TIME_H_
