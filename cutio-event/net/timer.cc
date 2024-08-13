// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

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
