/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/timer.h>

namespace cutio {
namespace event {

void Timer::Restart(Timestamp now) {
  if (repeat_) {
    expiration_ = AddTime(now, interval_);
  } else {
    expiration_ = Timestamp::Invalid();
  }
}

}  // namespace event
}  // namespace cutio
