#pragma once

#ifdef CWDEBUG
#include "debug_ostream_operators.h"
#endif

namespace wl {

template<typename WL_EVENTS_CONTAINER, auto SIGNAL_ENUM>
struct EventInfo
{
#define ALWAYS_FAIL_BECAUSE sizeof(WL_EVENTS_CONTAINER) == 0
  static_assert(ALWAYS_FAIL_BECAUSE, "You are using an unspecialized version of EventInfo!");
  using event_data_type = void;
};

#ifdef CWDEBUG
template<typename WLR_EVENTS_CONTAINER, auto SIGNAL_ENUM>
struct PrintType<EventInfo<WLR_EVENTS_CONTAINER, SIGNAL_ENUM>>
{
  void print_on(std::ostream& os) const
  {
    os << "wl::EventInfo<" << libcwd::type_info_of<WLR_EVENTS_CONTAINER>().demangled_name() << ", " << to_string(SIGNAL_ENUM) << ">";
  }
};
#endif

} // namespace wl
