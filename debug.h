#pragma once

#include "cwds/debug.h"

#ifdef CWDEBUG

#ifdef DEBUG_THREADPOOL_COLORS
// We use colors for debug output.
#include "threadpool/debug_colors.h"
#endif

// The xml submodule doesn't use utils.
#ifdef HAVE_UTILS_CONFIG_H

#include "utils/print_using.h"
#include "utils/QuotedList.h"
namespace libcwd {
// Allow using print_using and QuotedList inside Dout without the utils:: prefix.
using utils::print_using;
using utils::QuotedList;
} // namespace libcwd

// Add support for classes with a print_on method, defined in global namespace.
#include "utils/has_print_on.h"
// Add catch all for global namespace.
using utils::has_print_on::operator<<;

#endif // HAVE_UTILS_CONFIG_H

// Redefine Dout in order to include a `using namespace debug`.
#define WayArcDout(dc_namespace, debug_obj, cntrl, ...)         \
    LibcwDoutScopeBegin(dc_namespace, debug_obj, cntrl)         \
    using namespace ::debug;                                    \
    LibcwDoutStream << __VA_ARGS__;                             \
    LibcwDoutScopeEnd

#undef Dout
#define Dout(cntrl, data) \
    WayArcDout(LIBCWD_DEBUGCHANNELS, ::libcwd::libcw_do, cntrl, data)

// Same for DoutFatal.
#define WayArcDoutFatal(dc_namespace, debug_obj, cntrl, ...)    \
    LibcwDoutFatalScopeBegin(dc_namespace, debug_obj, cntrl)    \
    using namespace ::debug;                                    \
    LibcwDoutFatalStream << __VA_ARGS__;                        \
    LibcwDoutFatalScopeEnd

#undef DoutFatal
#define DoutFatal(cntrl, data) \
    WayArcDoutFatal(LIBCWD_DEBUGCHANNELS, ::libcwd::libcw_do, cntrl, data)

// Same for DoutEntering.
#undef DoutEntering
#define DoutEntering(cntrl, ...)                                        \
    int __cwds_debug_indentation = 2;                                   \
    LibcwDoutScopeBegin(DEBUGCHANNELS, ::libcwd::libcw_do, cntrl)       \
    using namespace ::debug;                                            \
    LibcwDoutStream << "Entering " << __VA_ARGS__;                      \
    LibcwDoutScopeEnd;                                                  \
    NAMESPACE_DEBUG::Indent __cwds_debug_indent(__cwds_debug_indentation);

// Same for Debug.
#define WayArcDebug(dc_namespace, STATEMENTS...)                \
    do {                                                        \
      using namespace ::libcwd;                                 \
      using namespace dc_namespace;                             \
      using namespace ::debug;                                  \
      {                                                         \
        STATEMENTS;                                             \
      }                                                         \
    } while(0)

#undef Debug
#define Debug(STATEMENTS...) \
    WayArcDebug(LIBCWD_DEBUGCHANNELS, STATEMENTS)

#if CWDEBUG_LOCATION
#include <libcwd/type_info.h>
#else
#include <typeinfo>
#endif

namespace debug {

// Initialize debugging code. Must be called at the very start of main().
void init();

// A helper class used to write the demangled type of T to an ostream.
// This is put here so that it can be specialized for certain classes
// without the need to include debug_ostream_operators.h.
template<typename T>
struct PrintType
{
  void print_on(std::ostream& os) const
  {
#if CWDEBUG_LOCATION
    os << libcwd::type_info_of<T>().demangled_name();
#else
    os << typeid(T).name();
#endif
  }
};

} // namespace debug

#endif // CWDEBUG
