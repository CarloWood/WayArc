#pragma once

#include "utils/has_print_on.h"

namespace wl {
// This class defines a print_on method.
using utils::has_print_on::operator<<;

template<typename T>
struct PrintType
{
  void print_on(std::ostream& os) const
  {
    os << libcwd::type_info_of<T>().demangled_name();
  }
};

template<typename T>
PrintType<T> print_type()
{
  return {};
}

} // namespace wl
