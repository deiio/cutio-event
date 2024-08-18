/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */


#ifndef CUTIO_EVENT_BASE_TYPES_H_
#define CUTIO_EVENT_BASE_TYPES_H_

#include <stdint.h>
#include <ext/vstring.h>
#include <ext/vstring_fwd.h>

namespace cutio {
namespace event {

typedef __gnu_cxx::__sso_string string;

// Taken from google-protobuf stubs/common.h
//
// Use implicit_cast as a safe version of static_cast or const_cast
// for upcasting in the type hierarchy (i.e. casting a pointer to Foo
// to a pointer to SuperclassOfFoo or casting a pointer to Foo to
// a const pointer to Foo).
// When you use implicit_cast, the compiler checks that the cast is safe.
// Such explicit implicit_casts are necessary in surprisingly many
// situations where C++ demands an exact type match instead of an
// argument type convertable to a target type.
//
// The From type can be inferred, so the preferred syntax for using
// implicit_cast is the same as for static_cast etc.:
//
//   implicit_cast<ToType>(expr)
//
// implicit_cast would have been part of the C++ standard library,
// but the proposal was submitted too late.  It will probably make
// its way into the language in the future.
template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_TYPES_H_
