/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_BASE_UTC_TIME_H_
#define CUTIO_EVENT_BASE_UTC_TIME_H_

#include <cutio-event/base/copyable.h>
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
class Timestamp : public copyable {
 public:
  // Constructs an Invalid Timestamp.
  Timestamp();

  // Constructs a Timestamp at specific time.
  explicit Timestamp(int64_t microSecondsSinceEpoch);

  // Default copy/assignment are okay.

  string ToString() const;
  string ToFormattedString() const;

  bool Valid() const {
    return microSecondsSinceEpoch_ > 0;
  }

  bool Before(Timestamp rhs) const {
    return microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
  }

  bool After(Timestamp rhs) const {
    return microSecondsSinceEpoch_ > rhs.microSecondsSinceEpoch_;
  }

  bool Equals(Timestamp rhs) const {
    return microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
  }

  // for internal usage.
  int64_t MicroSecondsSinceEpoch() const {
    return microSecondsSinceEpoch_;
  }

  // Get time of Now.
  static Timestamp Now();
  static Timestamp Invalid();
  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.Before(rhs);
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.Equals(rhs);
}

/**
 * Gets time difference of two timestamp, result in seconds.
 *
 * @param high, low
 * @return (high - low) in seconds
 * @c double has 52-bit precision, enough for one-microsecond
 * resolution for next 100 years.
 */
inline double TimeDifference(Timestamp high, Timestamp low) {
  auto diff = high.MicroSecondsSinceEpoch() - low.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

/**
 * Add @c seconds to given timestamp.
 *
 * @param timestamp, seconds
 * @return timestamp + seconds at Timestamp
 */
inline Timestamp AddTime(Timestamp timestamp, double seconds) {
  auto delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.MicroSecondsSinceEpoch() + delta);
}

}  // namespace event
}  // namespace cutio

#endif // CUTIO_EVENT_BASE_UTC_TIME_H_
