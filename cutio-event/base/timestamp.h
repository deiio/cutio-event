/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
