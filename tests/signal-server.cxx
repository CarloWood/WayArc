#include "sys.h"
#include "events/Events.h"
#include "wl/Listener.h"
#include "wayland-server-core.h"
#include "debug.h"

extern "C" {
struct wl_xyz_type {
  using data_type = wl_xyz_type;
  static constexpr bool one_shot = true;

  int magic_{0x123456};

  wl_xyz_type() { }
  wl_xyz_type(data_type*) { }
};

// wl_data_type
wl_signal xyz_event;
} // extern "C"

std::ostream& operator<<(std::ostream& os, wl_xyz_type const& xyz)
{
  os << '{';
  os << "magic_:0x" << std::hex << xyz.magic_;
  os << '}';
  return os;
}

using new_input_listener_t = wl::Listener<wl_xyz_type>;

struct tinywl_server
{
  new_input_listener_t new_input_;

  // Callback for new_input_.
  void new_input(wl_xyz_type const& device)
  {
    DoutEntering(dc::notice, "tinywl_server::new_input(" << device << ")");
  }
};

int main()
{
  Debug(NAMESPACE_DEBUG::init());

  Dout(dc::notice, "This is signal-server!");

  tinywl_server server;

  auto handle = server.new_input_.request(server, &tinywl_server::new_input);

  // External trigger happening.
  {
    wl_signal_init(&xyz_event);
    server.new_input_.init(&xyz_event);      // Can only do this after xyz_event was initialized.

    wl_xyz_type actual_data;
    wl_signal_emit(&xyz_event, &actual_data);
  }

  // Clean up event registration.
  handle.cancel();
}
