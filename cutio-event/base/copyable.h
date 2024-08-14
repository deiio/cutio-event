// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#ifndef CUTIO_EVENT_BASE_COPYABLE_H_
#define CUTIO_EVENT_BASE_COPYABLE_H_

namespace cutio {
namespace event {

/**
 * A tag class emphasises the objects are copyable.
 * The empty base class optimization applies.
 */
class copyable {
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_COPYABLE_H_
