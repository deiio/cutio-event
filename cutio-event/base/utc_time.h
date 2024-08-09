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
 * Time stamp in UTC.
 *
 * This class is immutable.
 * It's recommended to pass it by value, since it's passed in register
 * on x64.
 */
class UtcTime {
 public:
  // Constructs an invalid UtcTime.
  UtcTime();

  // Constructs a UtcTime at specific time.
  explicit UtcTime(int64_t microSecondsSinceEpoch);

  // Default copy/assignment are okay.

  string toString() const;

  bool valid() const {
    return microSecondsSinceEpoch_ > 0;
  }

  bool before(UtcTime rhs) const {
    return microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
  }

  bool after(UtcTime rhs) const {
    return microSecondsSinceEpoch_ > rhs.microSecondsSinceEpoch_;
  }

  bool equals(UtcTime rhs) const {
    return microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
  }

  // for internal usage.
  int64_t microSecondsSinceEpoch() const {
    return microSecondsSinceEpoch_;
  }

  static UtcTime now();
  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(UtcTime lhs, UtcTime rhs) {
  return lhs.before(rhs);
}

inline bool operator==(UtcTime lhs, UtcTime rhs) {
  return lhs.equals(rhs);
}

/**
 * Gets time difference of two timestamp, result in seconds.
 *
 * @param high, low
 * @return (high - low) in seconds
 */
inline double timeDifference(UtcTime high, UtcTime low) {
  auto diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / UtcTime::kMicroSecondsPerSecond;
}

}  // namespace event
}  // namespace cutio

#endif // CUTIO_EVENT_BASE_UTC_TIME_H_
