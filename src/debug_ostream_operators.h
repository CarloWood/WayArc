#pragma once

#ifndef CWDEBUG
#error "Don't include debug_ostream_operators.h unless CWDEBUG is defined please."
#endif

#include "utils/has_print_on.h"
#include <utility>

namespace debug {
// In order to be able to write a PrintType to an ostream.
using utils::has_print_on::operator<<;

template<typename T>
constexpr PrintType<T> print_type;

} // namespace debug
