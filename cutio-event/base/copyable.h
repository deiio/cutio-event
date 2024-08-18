/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

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
