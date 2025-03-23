#pragma once

#include "wl/EventInfo.h"
#ifdef CWDEBUG
#include "utils/has_print_on.h"
#include "utils/print_pointer.h"
#endif

namespace wl {

#ifdef CWDEBUG
// This class defines a print_on method.
using utils::has_print_on::operator<<;
#endif

template<auto SIGNAL_ENUM>
class EventType
{
 public:
  static constexpr auto signal_enum = SIGNAL_ENUM;
  using data_type = typename wl::EventInfo<SIGNAL_ENUM>::event_data_type;
  static constexpr bool one_shot = false;

 private:
  data_type* data_;

 public:
  EventType(data_type* data) : data_(data) { }

  data_type* operator->() const { return data_; }
  operator data_type*() const { return data_; }

#ifdef CWDEBUG
  void print_on(std::ostream& os) const;
#endif
};

#ifdef CWDEBUG
template<auto SIGNAL_ENUM>
void EventType<SIGNAL_ENUM>::print_on(std::ostream& os) const
{
  os << '{';
  os << "data_:" << utils::print_pointer(data_);
  os << '}';
}

template<auto SIGNAL_ENUM>
struct PrintType<EventType<SIGNAL_ENUM>>
{
  void print_on(std::ostream& os) const
  {
    os << "wl::EventType<" << to_string(SIGNAL_ENUM) << ">";
  }
};
#endif

} // namespace wl
