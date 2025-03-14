#pragma once

#include <wayland-server-core.h>

namespace wl {

class Listener : public wl_listener
{
 public:
  void register_with(struct wl_signal& signal, int)
  {

     //   server.new_input.notify = server_new_input;
     //   wl_signal_add(&server.backend->events.new_input, &server.new_input);
  }
};

} // namespace wl
