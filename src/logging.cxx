#include "sys.h"
#include <cstdio>
#include <stdexcept>
#include <vector>
#include "debug.h"

#if defined(CWDEBUG) && !defined(DOXYGEN)
NAMESPACE_DEBUG_CHANNELS_START
channel_ct wlroots("WLROOTS");
NAMESPACE_DEBUG_CHANNELS_END
#endif

extern "C" {
#include "wlr/util/log.h"
#include <stdarg.h>

static void log_libcwd(enum wlr_log_importance verbosity, char const* fmt, va_list args);
} // extern "C"

void write_formatted(std::ostream& os, char const* format, va_list args)
{
  thread_local static char buffer[1024];
  va_list args_copy;
  va_copy(args_copy, args);
  int len = std::vsnprintf(buffer, sizeof(buffer), format, args_copy);
  va_end(args_copy);
  if (len < 0)
    throw std::runtime_error("Formatting error");
  else if (static_cast<size_t>(len) < sizeof(buffer))
    os.rdbuf()->sputn(buffer, len);
  else
  {
    std::vector<char> large_buffer(len + 1);
    std::vsnprintf(large_buffer.data(), large_buffer.size(), format, args);
    os.rdbuf()->sputn(large_buffer.data(), len);
  }
}

// Call for debug output from wlroots.
static void log_libcwd(enum wlr_log_importance verbosity, char const* fmt, va_list args)
{
  if (verbosity > wlr_log_get_verbosity())
    return;

  unsigned c = (verbosity < WLR_LOG_IMPORTANCE_LAST) ? verbosity : WLR_LOG_IMPORTANCE_LAST - 1;

  static char const* verbosity_colors[] = {
    "",
    "\x1B[1;31m", // ERROR
    "\x1B[1;34m", // INFO
    "\x1B[1;90m", // DEBUG
  };

  LibcwDoutScopeBegin(LIBCWD_DEBUGCHANNELS, ::libcwd::libcw_do, dc::wlroots)
    LibcwDoutStream << verbosity_colors[c];
    write_formatted(LibcwDoutStream, fmt, args);
    LibcwDoutStream << "\x1B[0m";
  LibcwDoutScopeEnd;
}

namespace debug {

void init()
{
  Debug(NAMESPACE_DEBUG::init());
  wlr_log_init(WLR_DEBUG, &log_libcwd);
}

} // namespace debug
