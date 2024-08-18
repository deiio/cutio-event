/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_TIMER_ID_H_
#define CUTIO_EVENT_NET_TIMER_ID_H_

#include <cutio-event/base/copyable.h>

namespace cutio {
namespace event {

class Timer;

/**
 * An opaque identifier, for canceling Timer.
 */
class TimerId : public copyable {
 public:
  explicit TimerId(Timer* timer)
    : value_(timer) {}

 private:
  Timer* value_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TIMER_ID_H_