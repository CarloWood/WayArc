#pragma once

#ifdef CWDEBUG
#include "debug_ostream_operators.h"
#endif

namespace wl {

template<auto SIGNAL_ENUM>
struct EventInfo
{
#define ALWAYS_FAIL_BECAUSE sizeof(SIGNAL_ENUM) == 0
  static_assert(ALWAYS_FAIL_BECAUSE, "You are using an unspecialized version of EventInfo!");
  using event_data_type = void;
};

#ifdef CWDEBUG
template<auto SIGNAL_ENUM>
struct PrintType<EventInfo<SIGNAL_ENUM>>
{
  void print_on(std::ostream& os) const
  {
    os << "wl::EventInfo<" << to_string(SIGNAL_ENUM) << ">";
  }
};
#endif

} // namespace wl
