// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_BASE_NONCOPYABLE_H_
#define CUTIO_EVENT_BASE_NONCOPYABLE_H_

namespace cutio {
namespace event {

class noncopyable {
 public:
  noncopyable(const noncopyable&) = delete;
  const noncopyable& operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_NONCOPYABLE_H_