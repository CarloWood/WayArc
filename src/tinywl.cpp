#include "sys.h"

#include <cassert>
#include <cstdbool>
#include <cstdlib>
#include <ctime>
#include <list>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <wayland-server-core.h>

// wlroots doesn't support inclusion from C++?!
extern "C" {
#define static
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>
} // extern "C"

#include <xkbcommon/xkbcommon.h>

#include "wlr/Scene.h"
#include "wlr/EventClient.h"
#include "wlr/Output.h"
#include "wlr/XdgToplevel.h"
#include "wlr/Surface.h"
#include "wlr/Backend.h"
#include "wlr/XdgShell.h"
#include "wlr/Cursor.h"
#include "wlr/Seat.h"
#include "wlr/XdgPopup.h"
#include "wlr/Keyboard.h"
#include "wlr/InputDevice.h"
#include "utils/AIAlert.h"
#include "debug.h"
#ifdef CWDEBUG
#include "utils/debug_ostream_operators.h"
#endif

std::ostream& operator<<(std::ostream& os, struct wlr_xdg_toplevel const& xdg_toplevel)
{
  os << "TODO: print wlr_xdg_toplevel";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_input_device const& input_device)
{
  os << "TODO: print wlr_input_device";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_xdg_popup const& xdg_popup)
{
  os << "TODO: print wlr_xdg_popup";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_pointer_motion_event const& motion_event)
{
  os << "TODO: print wlr_pointer_motion_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_pointer_motion_absolute_event const& motion_absolute_event)
{
  os << "TODO: print wlr_pointer_motion_absolute_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_pointer_button_event const& button_event)
{
  os << "TODO: print wlr_pointer_button_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_pointer_axis_event const& axis_event)
{
  os << "TODO: print wlr_pointer_axis_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_cursor const& cursor)
{
  os << "TODO: print wlr_cursor";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_seat_pointer_request_set_cursor_event const& seat_request_set_cursor_event)
{
  os << "TODO: print wlr_seat_pointer_request_set_cursor_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_seat_request_set_selection_event const& request_selection_event)
{
  os << "TODO: print wlr_seat_request_set_selection_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_output const& output)
{
  os << "TODO: print wlr_output";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_output_event_request_state const& output_event_request_state)
{
  os << "TODO: print wlr_output_event_request_state";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_keyboard const& keyboard)
{
  os << "TODO: print wlr_keyboard";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_keyboard_key_event const& keyboard)
{
  os << "TODO: print wlr_keyboard_key_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_surface const& keyboard)
{
  os << "TODO: print wlr_surface";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_xdg_toplevel_move_event const& keyboard)
{
  os << "TODO: print wlr_xdg_toplevel_move_event";
  return os;
}

std::ostream& operator<<(std::ostream& os, struct wlr_xdg_toplevel_resize_event const& keyboard)
{
  os << "TODO: print wlr_xdg_toplevel_resize_event";
  return os;
}

/* For brevity's sake, struct members are annotated where they are used. */
enum tinywl_cursor_mode {
	TINYWL_CURSOR_PASSTHROUGH,
	TINYWL_CURSOR_MOVE,
	TINYWL_CURSOR_RESIZE,
};

class TinywlServer;
class TinywlKeyboard;

class TinywlToplevel : public wlr::EventClient
{
 public:
  TinywlServer* server_;

 private:
 public: // FIXME
  struct wlr_scene_tree* scene_tree_;

 public:
  wlr::XdgToplevel wlr_xdg_toplevel_;
  void xdg_toplevel_destroy(wlr::XdgToplevel::destroy::event_type const& event_type);
  void xdg_toplevel_request_move(wlr::XdgToplevel::request_move::event_type const& event_type);
  void xdg_toplevel_request_resize(wlr::XdgToplevel::request_resize::event_type const& event_type);
  void xdg_toplevel_request_maximize(wlr::XdgToplevel::request_maximize::event_type const& event_type);
  void xdg_toplevel_request_fullscreen(wlr::XdgToplevel::request_fullscreen::event_type const& event_type);

  wlr::Surface wlr_xdg_toplevel_base_surface_;                                  // wlr_xdg_toplevel_->base->surface
  void surface_map(wlr::Surface::map::event_type const& event_type);
  void surface_unmap(wlr::Surface::unmap::event_type const& event_type);
  void surface_commit(wlr::Surface::commit::event_type const& event_type);

 public:
  TinywlToplevel(TinywlServer* server, struct wlr_xdg_toplevel* wlr_xdg_toplevel) :
    server_(server), wlr_xdg_toplevel_(wlr_xdg_toplevel), wlr_xdg_toplevel_base_surface_(wlr_xdg_toplevel_->base->surface)
  {
    register_events();
  }

  ~TinywlToplevel()
  {
    cancel_events();
  }

  void focus();

 private:
  void begin_interactive(enum tinywl_cursor_mode mode, uint32_t edges);

  void register_events()
  {
    register_event(wlr_xdg_toplevel_, &TinywlToplevel::xdg_toplevel_destroy);
    register_event(wlr_xdg_toplevel_, &TinywlToplevel::xdg_toplevel_request_move);
    register_event(wlr_xdg_toplevel_, &TinywlToplevel::xdg_toplevel_request_resize);
    register_event(wlr_xdg_toplevel_, &TinywlToplevel::xdg_toplevel_request_maximize);
    register_event(wlr_xdg_toplevel_, &TinywlToplevel::xdg_toplevel_request_fullscreen);
    register_event(wlr_xdg_toplevel_base_surface_, &TinywlToplevel::surface_map);
    register_event(wlr_xdg_toplevel_base_surface_, &TinywlToplevel::surface_unmap);
    register_event(wlr_xdg_toplevel_base_surface_, &TinywlToplevel::surface_commit);
  }
};

class TinywlServer : public wlr::EventClient
{
 private:
  struct wl_display* wl_display_{};

  wlr::Backend wlr_backend_;
  void backend_new_input(wlr::Backend::new_input::event_type const& event_type);
  void backend_new_output(wlr::Backend::new_output::event_type const& event_type);

  struct wlr_renderer* renderer_{};
  struct wlr_allocator* allocator_{};
  //wlr::Scene scene_;
 public: // FIXME
  wlr_scene* scene_;
  struct wlr_scene_output_layout* scene_layout_;
 private:

  wlr::XdgShell wlr_xdg_shell_;
  void xdg_shell_new_toplevel(wlr::XdgShell::new_toplevel::event_type const& event_type);
  void xdg_shell_new_popup(wlr::XdgShell::new_popup::event_type const& event_type);

 public:
  std::list<TinywlToplevel*> toplevels_;

  wlr::Cursor wlr_cursor_;
  void cursor_motion(wlr::Cursor::motion::event_type const& event_type);
  void cursor_motion_absolute(wlr::Cursor::motion_absolute::event_type const& event_type);
  void cursor_button(wlr::Cursor::button::event_type const& event_type);
  void cursor_axis(wlr::Cursor::axis::event_type const& event_type);
  void cursor_frame(wlr::Cursor::frame::event_type const& event_type);

 public:
  struct wlr_xcursor_manager* cursor_mgr_{};

  wlr::Seat wlr_seat_;
  void seat_request_set_cursor(wlr::Seat::request_set_cursor::event_type const& event_type);
  void seat_request_set_selection(wlr::Seat::request_set_selection::event_type const& event_type);

  struct wl_list keyboards_;
  enum tinywl_cursor_mode cursor_mode_;
  TinywlToplevel* grabbed_toplevel_;
  double grab_x_;
  double grab_y_;
  struct wlr_box grab_geobox_;
  uint32_t resize_edges_;

  struct wlr_output_layout* output_layout_;
  struct wl_list outputs_;

 public:
  TinywlServer(char const* startup_cmd)
  {
    initialize_display();
    initialize_backend();
    initialize_renderer();
    initialize_allocator();
    initialize_output_layout();
    initialize_scene();
    initialize_xdg_shell();
    initialize_cursor();
    initialize_seat();

    // This creates some hands-off wlroots interfaces. The compositor is
    // necessary for clients to allocate surfaces, the subcompositor allows to
    // assign the role of subsurfaces to surfaces and the data device manager
    // handles the clipboard. Each of these wlroots interfaces has room for you
    // to dig your fingers in and play with their behavior if you want. Note that
    // the clients cannot set the selection directly without compositor approval,
    // see the handling of the seat_request_set_selection event.
    wlr_compositor_create(wl_display_, 5, renderer_);
    wlr_subcompositor_create(wl_display_);
    wlr_data_device_manager_create(wl_display_);

    create_wayland_socket();

    // Run the startup command if requested.
    if (startup_cmd)
    {
      if (fork() == 0)
        execl("/bin/sh", "/bin/sh", "-c", startup_cmd, (void*)nullptr);
    }
  }

  // Can't move this because we're storing pointers to EventClient's that are members of this object.
  TinywlServer(TinywlServer&&) = delete;

  ~TinywlServer()
  {
    cancel_events();

    if (wl_display_)
      wl_display_destroy_clients(wl_display_);
    // No need to remove signal listeners with wl::Listener - they clean up automatically.
    wlr_scene_node_destroy(&scene_->tree.node);
    wlr_xcursor_manager_destroy(cursor_mgr_);
    if (wlr_cursor_)
      wlr_cursor_destroy(wlr_cursor_);
    wlr_allocator_destroy(allocator_);
    wlr_renderer_destroy(renderer_);
    wlr_backend_destroy(wlr_backend_);
    if (wl_display_)
      wl_display_destroy(wl_display_);
  }

  void run()
  {
    // This blocks until the compositors is terminated.
    wl_display_run(wl_display_);
  }

 private:
  void new_keyboard(struct wlr_input_device* device);
  void new_pointer(struct wlr_input_device* device);

  friend class TinywlKeyboard;
  bool handle_keybinding(xkb_keysym_t sym);

  friend class TinywlToplevel;
  void reset_cursor_mode();

  void process_cursor_move();
  void process_cursor_resize();
  void process_cursor_motion(uint32_t time);
  TinywlToplevel* desktop_toplevel_at(double lx, double ly, struct wlr_surface** surface, double* sx, double* sy);

  void initialize_display()
  {
    // The Wayland display is managed by libwayland. It handles accepting
    // clients from the Unix socket, manging Wayland globals, and so on.
    wl_display_ = wl_display_create();
  }

  void initialize_backend()
  {
    // The backend is a wlroots feature which abstracts the underlying input and
    // output hardware. The autocreate option will choose the most suitable
    // backend based on the current environment, such as opening an X11 window
    // if an X11 server is running.
    wlr_backend_ = wlr_backend_autocreate(wl_display_get_event_loop(wl_display_), nullptr);
    if (wlr_backend_ == nullptr)
      THROW_ALERT("Failed to create wlr_backend");

    // Calls to backend_new_input add the new input to this list if it is a keyboard.
    wl_list_init(&keyboards_);

    // Calls to backend_new_output add the new output to this list.
    wl_list_init(&outputs_);

    // Configure a listener to be notified when new inputs are available on the backend.
    register_event(wlr_backend_, &TinywlServer::backend_new_input);

    // Configure a listener to be notified when new outputs are available on the backend.
    register_event(wlr_backend_, &TinywlServer::backend_new_output);
  }

  void initialize_renderer()
  {
    // Autocreates a renderer, either Pixman, GLES2 or Vulkan for us. The user
    // can also specify a renderer using the WLR_RENDERER env var.
    // The renderer is responsible for defining the various pixel formats it
    // supports for shared memory, this configures that for clients.
    renderer_ = wlr_renderer_autocreate(wlr_backend_);
    if (renderer_ == nullptr)
      THROW_ALERT("failed to create wlr_renderer");
    wlr_renderer_init_wl_display(renderer_, wl_display_);
  }

  void initialize_allocator()
  {
    /* Autocreates an allocator for us.
     * The allocator is the bridge between the renderer and the backend. It
     * handles the buffer creation, allowing wlroots to render onto the
     * screen */
    allocator_ = wlr_allocator_autocreate(wlr_backend_, renderer_);
    if (allocator_ == nullptr)
      THROW_ALERT("failed to create wlr_allocator");
  }

  void initialize_output_layout()
  {
    // Creates an output layout, which a wlroots utility for working with an
    // arrangement of screens in a physical layout.
    output_layout_ = wlr_output_layout_create(wl_display_);
  }

  void initialize_scene()
  {
    // Create a scene graph. This is a wlroots abstraction that handles all
    // rendering and damage tracking. All the compositor author needs to do
    // is add things that should be rendered to the scene graph at the proper
    // positions and then call wlr_scene_output_commit() to render a frame if
    // necessary.
    scene_ = wlr_scene_create();
    scene_layout_ = wlr_scene_attach_output_layout(scene_, output_layout_);
  }

  void initialize_xdg_shell()
  {
    // Set up xdg-shell version 3. The xdg-shell is a Wayland protocol which is
    // used for application windows. For more detail on shells, refer to
    // https://drewdevault.com/2018/07/29/Wayland-shells.html.
    wlr_xdg_shell_ = wlr_xdg_shell_create(wl_display_, 3);
    register_event(wlr_xdg_shell_, &TinywlServer::xdg_shell_new_toplevel);
    register_event(wlr_xdg_shell_, &TinywlServer::xdg_shell_new_popup);
  }

  void initialize_cursor()
  {
    // Creates a cursor, which is a wlroots utility for tracking the cursor
    // image shown on screen.
    wlr_cursor_ = wlr_cursor_create();
    wlr_cursor_attach_output_layout(wlr_cursor_, output_layout_);

    // Creates an xcursor manager, another wlroots utility which loads up
    // Xcursor themes to source cursor images from and makes sure that cursor
    // images are available at all scale factors on the screen (necessary for
    // HiDPI support).
    cursor_mgr_ = wlr_xcursor_manager_create(nullptr, 24);

    // wlr_cursor *only* displays an image on screen. It does not move around
    // when the pointer moves. However, we can attach input devices to it, and
    // it will generate aggregate events for all of them. In these events, we
    // can choose how we want to process them, forwarding them to clients and
    // moving the cursor around. More detail on this process is described in
    // https://drewdevault.com/2018/07/17/Input-handling-in-wlroots.html.
    //
    // And more comments are sprinkled throughout the notify functions above.
    cursor_mode_ = TINYWL_CURSOR_PASSTHROUGH;

    register_event(wlr_cursor_, &TinywlServer::cursor_motion);
    register_event(wlr_cursor_, &TinywlServer::cursor_motion_absolute);
    register_event(wlr_cursor_, &TinywlServer::cursor_button);
    register_event(wlr_cursor_, &TinywlServer::cursor_axis);
    register_event(wlr_cursor_, &TinywlServer::cursor_frame);
  }

  void initialize_seat()
  {
    // Configures a seat, which is a single "seat" at which a user sits and
    // operates the computer. This conceptually includes up to one keyboard,
    // pointer, touch, and drawing tablet device. We also rig up a listener to
    // let us know when new input devices are available on the backend.
    wlr_seat_ = wlr_seat_create(wl_display_, "seat0");

    register_event(wlr_seat_, &TinywlServer::seat_request_set_cursor);
    register_event(wlr_seat_, &TinywlServer::seat_request_set_selection);
  }

  void create_wayland_socket()
  {
    // Add a Unix socket to the Wayland display.
    char const* socket = wl_display_add_socket_auto(wl_display_);
    if (!socket)
      THROW_ALERT("Failed to create Wayland UNIX socket");

    // Start the backend. This will enumerate outputs and inputs, become the DRM master, etc.
    if (!wlr_backend_start(wlr_backend_))
      THROW_ALERT("Failed to start backend");

    // Set the WAYLAND_DISPLAY environment variable to our socket.
    setenv("WAYLAND_DISPLAY", socket, true);

    wlr_log(WLR_INFO, "Running Wayland compositor on WAYLAND_DISPLAY=%s", socket);
  }
};

class TinywlOutput : public wlr::EventClient
{
 public:
  struct wl_list link;

 private:
  TinywlServer* server_;

  wlr::Output wlr_output_;
  void output_frame(wlr::Output::frame::event_type const& output);
  void output_request_state(wlr::Output::request_state::event_type const& output_event_request_state);
  void output_destroy(wlr::Output::destroy::event_type const& output);

 public:
  TinywlOutput(TinywlServer* server, struct wlr_output* wlr_output) : server_(server), wlr_output_(wlr_output)
  {
    register_events();
  }

  ~TinywlOutput()
  {
    cancel_events();
    wl_list_remove(&link);
  }

 private:
  void register_events()
  {
    register_event(wlr_output_, &TinywlOutput::output_frame);
    register_event(wlr_output_, &TinywlOutput::output_request_state);
    register_event(wlr_output_, &TinywlOutput::output_destroy);
  }
};

class TinywlPopup : public wlr::EventClient
{
 private:
  wlr::XdgPopup wlr_xdg_popup_;
  void xdg_popup_destroy(wlr::XdgPopup::destroy::event_type const& event_type);

  wlr::Surface wlr_xdg_popup_base_surface_;                     // wlr_xdg_popup_->base->surface
  void surface_commit(wlr::Surface::commit::event_type const& event_type);

 public:
  TinywlPopup(struct wlr_xdg_popup* wlr_xdg_popup) : wlr_xdg_popup_(wlr_xdg_popup), wlr_xdg_popup_base_surface_(wlr_xdg_popup_->base->surface)
  {
    register_events();
  }

  ~TinywlPopup()
  {
    cancel_events();
  }

 private:
  void register_events()
  {
    register_event(wlr_xdg_popup_, &TinywlPopup::xdg_popup_destroy);
    register_event(wlr_xdg_popup_base_surface_, &TinywlPopup::surface_commit);
  }
};

class TinywlKeyboard : public wlr::EventClient
{
 public:
  struct wl_list link;

 public:
  TinywlServer* server_;

  wlr::Keyboard wlr_keyboard_;
  void keyboard_modifiers(wlr::Keyboard::modifiers::event_type const& event_type);
  void keyboard_key(wlr::Keyboard::key::event_type const& event_type);

  wlr::InputDevice wlr_input_device_;
  void input_device_destroy(wlr::InputDevice::destroy::event_type const& event_type);

 public:
  TinywlKeyboard(TinywlServer* server, struct wlr_keyboard* wlr_keyboard) :
    server_(server), wlr_keyboard_(wlr_keyboard), wlr_input_device_(&wlr_keyboard->base)
  {
    register_events();
  }

  ~TinywlKeyboard()
  {
    cancel_events();
    wl_list_remove(&link);
  }

 public:
  void register_events()
  {
    register_event(wlr_keyboard_, &TinywlKeyboard::keyboard_modifiers);
    register_event(wlr_keyboard_, &TinywlKeyboard::keyboard_key);
    register_event(wlr_input_device_, &TinywlKeyboard::input_device_destroy);
  }
};

void TinywlToplevel::focus()
{
	/* Note: this function only deals with keyboard focus. */
	TinywlServer* server = server_;
	struct wlr_seat* seat = server->wlr_seat_;
	struct wlr_surface* prev_surface = seat->keyboard_state.focused_surface;
	struct wlr_surface* surface = wlr_xdg_toplevel_->base->surface;
	if (prev_surface == surface) {
		/* Don't re-focus an already focused surface. */
		return;
	}
	if (prev_surface) {
		/*
		 * Deactivate the previously focused surface. This lets the client know
		 * it no longer has focus and the client will repaint accordingly, e.g.
		 * stop displaying a caret.
		 */
		struct wlr_xdg_toplevel* prev_toplevel =
			wlr_xdg_toplevel_try_from_wlr_surface(prev_surface);
		if (prev_toplevel != NULL) {
			wlr_xdg_toplevel_set_activated(prev_toplevel, false);
		}
	}
	struct wlr_keyboard* keyboard = wlr_seat_get_keyboard(seat);
	// Move the this toplevel to the front.
	wlr_scene_node_raise_to_top(&scene_tree_->node);
        auto toplevel_iter = std::find(server->toplevels_.begin(), server->toplevels_.end(), this);
        server->toplevels_.splice(server->toplevels_.begin(), server->toplevels_, toplevel_iter);

	/* Activate the new surface */
	wlr_xdg_toplevel_set_activated(wlr_xdg_toplevel_, true);
	/*
	 * Tell the seat to have the keyboard enter this surface. wlroots will keep
	 * track of this and automatically send key events to the appropriate
	 * clients without additional work on your part.
	 */
	if (keyboard != NULL) {
		wlr_seat_keyboard_notify_enter(seat, surface,
			keyboard->keycodes, keyboard->num_keycodes, &keyboard->modifiers);
	}
}

/* This event is raised when a modifier key, such as shift or alt, is
 * pressed. We simply communicate this to the client. */
void TinywlKeyboard::keyboard_modifiers(wlr::Keyboard::modifiers::event_type const& wlr_keyboard)
{
  /*
   * A seat can only have one keyboard, but this is a limitation of the
   * Wayland protocol - not wlroots. We assign all connected keyboards to the
   * same seat. You can swap out the underlying wlr_keyboard like this and
   * wlr_seat handles this transparently.
   */
  wlr_seat_set_keyboard(server_->wlr_seat_, wlr_keyboard);
  /* Send modifiers to the client. */
  wlr_seat_keyboard_notify_modifiers(server_->wlr_seat_, &wlr_keyboard->modifiers);
}

bool TinywlServer::handle_keybinding(xkb_keysym_t sym) {
	/*
	 * Here we handle compositor keybindings. This is when the compositor is
	 * processing keys, rather than passing them on to the client for its own
	 * processing.
	 *
	 * This function assumes Alt is held down.
	 */
	switch (sym) {
	case XKB_KEY_Escape:
		wl_display_terminate(wl_display_);
		break;
	case XKB_KEY_F1:
              {
		/* Cycle to the next toplevel */
		if (toplevels_.size() < 2) {
			break;
		}
                auto next_toplevel_iter = std::prev(toplevels_.end());
		(*next_toplevel_iter)->focus();
		break;
              }
	default:
		return false;
	}
	return true;
}

/* This event is raised when a key is pressed or released. */
void TinywlKeyboard::keyboard_key(wlr::Keyboard::key::event_type const& event_type)
{
  struct wlr_seat* seat = server_->wlr_seat_;

  /* Translate libinput keycode -> xkbcommon */
  uint32_t keycode = event_type->keycode + 8;
  /* Get a list of keysyms based on the keymap for this keyboard */
  xkb_keysym_t const* syms;
  int nsyms = xkb_state_key_get_syms(wlr_keyboard_->xkb_state, keycode, &syms);

  bool handled = false;
  uint32_t modifiers = wlr_keyboard_get_modifiers(wlr_keyboard_);
  if ((modifiers & WLR_MODIFIER_ALT) &&
                  event_type->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
          /* If alt is held down and this button was _pressed_, we attempt to
           * process it as a compositor keybinding. */
          for (int i = 0; i < nsyms; i++) {
                  handled = server_->handle_keybinding(syms[i]);
          }
  }

  if (!handled) {
          /* Otherwise, we pass it along to the client. */
          wlr_seat_set_keyboard(seat, wlr_keyboard_);
          wlr_seat_keyboard_notify_key(seat, event_type->time_msec, event_type->keycode, event_type->state);
  }
}

/* This event is raised by the keyboard base wlr_input_device to signal
 * the destruction of the wlr_keyboard. It will no longer receive events
 * and should be destroyed.
 */
void TinywlKeyboard::input_device_destroy(wlr::InputDevice::destroy::event_type const& event_type)
{
  delete this;   // FIXME
}

void TinywlServer::new_keyboard(struct wlr_input_device* device) {
	struct wlr_keyboard* wlr_keyboard = wlr_keyboard_from_input_device(device);

	TinywlKeyboard* keyboard = new TinywlKeyboard(this, wlr_keyboard);

	/* We need to prepare an XKB keymap and assign it to the keyboard. This
	 * assumes the defaults (e.g. layout = "us"). */
	struct xkb_context* context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	struct xkb_keymap* keymap = xkb_keymap_new_from_names(context, NULL,
		XKB_KEYMAP_COMPILE_NO_FLAGS);

	wlr_keyboard_set_keymap(wlr_keyboard, keymap);
	xkb_keymap_unref(keymap);
	xkb_context_unref(context);
	wlr_keyboard_set_repeat_info(wlr_keyboard, 25, 600);

	wlr_seat_set_keyboard(wlr_seat_, keyboard->wlr_keyboard_);

	// And add the keyboard to our list of keyboards.
	wl_list_insert(&keyboards_, &keyboard->link);
}

void TinywlServer::new_pointer(struct wlr_input_device* device) {
	/* We don't do anything special with pointers. All of our pointer handling
	 * is proxied through wlr_cursor. On another compositor, you might take this
	 * opportunity to do libinput configuration on the device to set
	 * acceleration, etc. */
	wlr_cursor_attach_input_device(wlr_cursor_, device);
}

void TinywlServer::backend_new_input(wlr::Backend::new_input::event_type const& device)
{
  /* This event is raised by the backend when a new input device becomes available. */
  switch (device->type)
  {
    case WLR_INPUT_DEVICE_KEYBOARD:
      new_keyboard(device);
      break;
    case WLR_INPUT_DEVICE_POINTER:
      new_pointer(device);
      break;
    default:
      break;
  }
  /* We need to let the wlr_seat know what our capabilities are, which is
   * communiciated to the client. In TinyWL we always have a cursor, even if
   * there are no pointer devices, so we always include that capability. */
  uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
  if (!wl_list_empty(&keyboards_))
    caps |= WL_SEAT_CAPABILITY_KEYBOARD;
  wlr_seat_set_capabilities(wlr_seat_, caps);
}

void TinywlServer::seat_request_set_cursor(wlr::Seat::request_set_cursor::event_type const& event)
{
  /* This event is raised by the seat when a client provides a cursor image */
  struct wlr_seat_client* focused_client = wlr_seat_->pointer_state.focused_client;
  /* This can be sent by any client, so we check to make sure this one is
   * actually has pointer focus first. */
  if (focused_client == event->seat_client) {
    /* Once we've vetted the client, we can tell the cursor to use the
     * provided surface as the cursor image. It will set the hardware cursor
     * on the output that it's currently on and continue to do so as the
     * cursor moves between outputs. */
    wlr_cursor_set_surface(wlr_cursor_, event->surface, event->hotspot_x, event->hotspot_y);
  }
}

void TinywlServer::seat_request_set_selection(wlr::Seat::request_set_selection::event_type const& event)
{
  /* This event is raised by the seat when a client wants to set the selection,
   * usually when the user copies something. wlroots allows compositors to
   * ignore such requests if they so choose, but in tinywl we always honor
   */
  wlr_seat_set_selection(wlr_seat_, event->source, event->serial);
}

TinywlToplevel* TinywlServer::desktop_toplevel_at(double lx, double ly, struct wlr_surface** surface, double* sx, double* sy)
{
	/* This returns the topmost node in the scene at the given layout coords.
	 * We only care about surface nodes as we are specifically looking for a
	 * surface in the surface tree of a TinywlToplevel. */
	struct wlr_scene_node* node = wlr_scene_node_at(&scene_->tree.node, lx, ly, sx, sy);
	if (node == NULL || node->type != WLR_SCENE_NODE_BUFFER) {
		return NULL;
	}
	struct wlr_scene_buffer* scene_buffer = wlr_scene_buffer_from_node(node);
	struct wlr_scene_surface* scene_surface =
		wlr_scene_surface_try_from_buffer(scene_buffer);
	if (!scene_surface) {
		return NULL;
	}

	*surface = scene_surface->surface;
	/* Find the node corresponding to the TinywlToplevel at the root of this
	 * surface tree, it is the only one for which we set the data field. */
	struct wlr_scene_tree* tree = node->parent;
	while (tree != NULL && tree->node.data == NULL) {
		tree = tree->node.parent;
	}
	return static_cast<TinywlToplevel*>(tree->node.data);
}

void TinywlServer::reset_cursor_mode() {
	// Reset the cursor mode to passthrough.
	cursor_mode_ = TINYWL_CURSOR_PASSTHROUGH;
	grabbed_toplevel_ = nullptr;
}

void TinywlServer::process_cursor_move()
{
	/* Move the grabbed toplevel to the new position. */
	TinywlToplevel* toplevel = grabbed_toplevel_;
	wlr_scene_node_set_position(&toplevel->scene_tree_->node,
		wlr_cursor_->x - grab_x_,
		wlr_cursor_->y - grab_y_);
}

void TinywlServer::process_cursor_resize() {
	/*
	 * Resizing the grabbed toplevel can be a little bit complicated, because we
	 * could be resizing from any corner or edge. This not only resizes the
	 * toplevel on one or two axes, but can also move the toplevel if you resize
	 * from the top or left edges (or top-left corner).
	 *
	 * Note that some shortcuts are taken here. In a more fleshed-out
	 * compositor, you'd wait for the client to prepare a buffer at the new
	 * size, then commit any movement that was prepared.
	 */
	TinywlToplevel* toplevel = grabbed_toplevel_;
	double border_x = wlr_cursor_->x - grab_x_;
	double border_y = wlr_cursor_->y - grab_y_;
	int new_left = grab_geobox_.x;
	int new_right = grab_geobox_.x + grab_geobox_.width;
	int new_top = grab_geobox_.y;
	int new_bottom = grab_geobox_.y + grab_geobox_.height;

	if (resize_edges_ & WLR_EDGE_TOP) {
		new_top = border_y;
		if (new_top >= new_bottom) {
			new_top = new_bottom - 1;
		}
	} else if (resize_edges_ & WLR_EDGE_BOTTOM) {
		new_bottom = border_y;
		if (new_bottom <= new_top) {
			new_bottom = new_top + 1;
		}
	}
	if (resize_edges_ & WLR_EDGE_LEFT) {
		new_left = border_x;
		if (new_left >= new_right) {
			new_left = new_right - 1;
		}
	} else if (resize_edges_ & WLR_EDGE_RIGHT) {
		new_right = border_x;
		if (new_right <= new_left) {
			new_right = new_left + 1;
		}
	}

	struct wlr_box* geo_box = &toplevel->wlr_xdg_toplevel_->base->geometry;
	wlr_scene_node_set_position(&toplevel->scene_tree_->node,
		new_left - geo_box->x, new_top - geo_box->y);

	int new_width = new_right - new_left;
	int new_height = new_bottom - new_top;
	wlr_xdg_toplevel_set_size(toplevel->wlr_xdg_toplevel_, new_width, new_height);
}

void TinywlServer::process_cursor_motion(uint32_t time) {
	/* If the mode is non-passthrough, delegate to those functions. */
	if (cursor_mode_ == TINYWL_CURSOR_MOVE) {
		process_cursor_move();
		return;
	} else if (cursor_mode_ == TINYWL_CURSOR_RESIZE) {
		process_cursor_resize();
		return;
	}

	/* Otherwise, find the toplevel under the pointer and send the event along. */
	double sx, sy;
	struct wlr_seat* seat = wlr_seat_;
	struct wlr_surface* surface = NULL;
	TinywlToplevel* toplevel = desktop_toplevel_at(wlr_cursor_->x, wlr_cursor_->y, &surface, &sx, &sy);
	if (!toplevel) {
		/* If there's no toplevel under the cursor, set the cursor image to a
		 * default. This is what makes the cursor image appear when you move it
		 * around the screen, not over any toplevels. */
		wlr_cursor_set_xcursor(wlr_cursor_, cursor_mgr_, "default");
	}
	if (surface) {
		/*
		 * Send pointer enter and motion events.
		 *
		 * The enter event gives the surface "pointer focus", which is distinct
		 * from keyboard focus. You get pointer focus by moving the pointer over
		 * a window.
		 *
		 * Note that wlroots will avoid sending duplicate enter/motion events if
		 * the surface has already has pointer focus or if the client is already
		 * aware of the coordinates passed.
		 */
		wlr_seat_pointer_notify_enter(seat, surface, sx, sy);
		wlr_seat_pointer_notify_motion(seat, time, sx, sy);
	} else {
		/* Clear pointer focus so future button events and such are not sent to
		 * the last client to have the cursor over it. */
		wlr_seat_pointer_clear_focus(seat);
	}
}

void TinywlServer::cursor_motion(wlr::Cursor::motion::event_type const& event)
{
  /* This event is forwarded by the cursor when a pointer emits a _relative_
   * pointer motion event (i.e. a delta) */
  /* The cursor doesn't move unless we tell it to. The cursor automatically
   * handles constraining the motion to the output layout, as well as any
   * special configuration applied for the specific input device which
   * generated the event. You can pass NULL for the device if you want to move
   * the cursor around without any input. */
  wlr_cursor_move(wlr_cursor_, &event->pointer->base, event->delta_x, event->delta_y);
  process_cursor_motion(event->time_msec);
}

void TinywlServer::cursor_motion_absolute(wlr::Cursor::motion_absolute::event_type const& event)
{
  /* This event is forwarded by the cursor when a pointer emits an _absolute_
   * motion event, from 0..1 on each axis. This happens, for example, when
   * wlroots is running under a Wayland window rather than KMS+DRM, and you
   * move the mouse over the window. You could enter the window from any edge,
   * so we have to warp the mouse there. There is also some hardware which
   * emits these events. */
  wlr_cursor_warp_absolute(wlr_cursor_, &event->pointer->base, event->x, event->y);
  process_cursor_motion(event->time_msec);
}

void TinywlServer::cursor_button(wlr::Cursor::button::event_type const& event)
{
  /* This event is forwarded by the cursor when a pointer emits a button
   * event. */
  /* Notify the client with pointer focus that a button press has occurred */
  wlr_seat_pointer_notify_button(wlr_seat_,
      event->time_msec, event->button, event->state);
  if (event->state == WL_POINTER_BUTTON_STATE_RELEASED) {
    /* If you released any buttons, we exit interactive move/resize mode. */
    reset_cursor_mode();
  } else {
    /* Focus that client if the button was _pressed_ */
    double sx, sy;
    struct wlr_surface* surface = NULL;
    TinywlToplevel* toplevel = desktop_toplevel_at(wlr_cursor_->x, wlr_cursor_->y, &surface, &sx, &sy);
    if (toplevel)
      toplevel->focus();
  }
}

void TinywlServer::cursor_axis(wlr::Cursor::axis::event_type const& event)
{
  /* This event is forwarded by the cursor when a pointer emits an axis event,
   * for example when you move the scroll wheel. */
  /* Notify the client with pointer focus of the axis event. */
  wlr_seat_pointer_notify_axis(wlr_seat_,
      event->time_msec, event->orientation, event->delta,
      event->delta_discrete, event->source, event->relative_direction);
}

void TinywlServer::cursor_frame(wlr::Cursor::frame::event_type const& data)
{
  /* This event is forwarded by the cursor when a pointer emits an frame
   * event. Frame events are sent after regular pointer events to group
   * multiple events together. For instance, two axis events may happen at the
   * same time, in which case a frame event won't be sent in between. */
  /* Notify the client with pointer focus of the frame event. */
  wlr_seat_pointer_notify_frame(wlr_seat_);
}

void TinywlOutput::output_frame(wlr::Output::frame::event_type const& UNUSED_ARG(output))
{
  /* This function is called every time an output is ready to display a frame,
   * generally at the output's refresh rate (e.g. 60Hz). */
  struct wlr_scene* scene = server_->scene_;

  struct wlr_scene_output* scene_output = wlr_scene_get_scene_output(scene, wlr_output_);

  /* Render the scene if needed and commit the output */
  wlr_scene_output_commit(scene_output, NULL);

  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  wlr_scene_output_send_frame_done(scene_output, &now);
}

void TinywlOutput::output_request_state(wlr::Output::request_state::event_type const& output_event_request_state)
{
  /* This function is called when the backend requests a new state for
   * the output. For example, Wayland and X11 backends request a new mode
   * when the output window is resized. */
  wlr_output_commit_state(wlr_output_, output_event_request_state->state);
}

void TinywlOutput::output_destroy(wlr::Output::destroy::event_type const& UNUSED_ARG(output))
{
  delete this; // FIXME
}

void TinywlServer::backend_new_output(wlr::Backend::new_output::event_type const& wlr_output)
{
  /* This event is raised by the backend when a new output (aka a display or
   * monitor) becomes available. */

  /* Configures the output created by the backend to use our allocator
   * and our renderer. Must be done once, before commiting the output */
  wlr_output_init_render(wlr_output, allocator_, renderer_);

  /* The output may be disabled, switch it on. */
  struct wlr_output_state state;
  wlr_output_state_init(&state);
  wlr_output_state_set_enabled(&state, true);

  /* Some backends don't have modes. DRM+KMS does, and we need to set a mode
   * before we can use the output. The mode is a tuple of (width, height,
   * refresh rate), and each monitor supports only a specific set of modes. We
   * just pick the monitor's preferred mode, a more sophisticated compositor
   * would let the user configure it. */
  struct wlr_output_mode* mode = wlr_output_preferred_mode(wlr_output);
  if (mode != NULL) {
    wlr_output_state_set_mode(&state, mode);
  }

  /* Atomically applies the new output state. */
  wlr_output_commit_state(wlr_output, &state);
  wlr_output_state_finish(&state);

  /* Allocates and configures our state for this output */
  TinywlOutput* output = new TinywlOutput(this, wlr_output);

  wl_list_insert(&outputs_, &output->link);

  /* Adds this to the output layout. The add_auto function arranges outputs
   * from left-to-right in the order they appear. A more sophisticated
   * compositor would let the user configure the arrangement of outputs in the
   * layout.
   *
   * The output layout utility automatically adds a wl_output global to the
   * display, which Wayland clients can see to find out information about the
   * output (such as DPI, scale factor, manufacturer, etc).
   */
  struct wlr_output_layout_output* l_output = wlr_output_layout_add_auto(output_layout_, wlr_output);
  struct wlr_scene_output* scene_output = wlr_scene_output_create(scene_, wlr_output);
  wlr_scene_output_layout_add_output(scene_layout_, l_output, scene_output);
}

/* The static functions were replaced by member functions above */

/* Called when the surface is mapped, or ready to display on-screen. */
void TinywlToplevel::surface_map(wlr::Surface::map::event_type const& data)
{
  server_->toplevels_.push_front(this);
  focus();
}

/* Called when the surface is unmapped, and should no longer be shown. */
void TinywlToplevel::surface_unmap(wlr::Surface::unmap::event_type const& data)
{
  /* Reset the cursor mode if the grabbed toplevel was unmapped. */
  if (this == server_->grabbed_toplevel_) {
    server_->reset_cursor_mode();
  }

  server_->toplevels_.remove(this);
}

/* Called when a new surface state is committed. */
void TinywlToplevel::surface_commit(wlr::Surface::commit::event_type const& data)
{
  if (wlr_xdg_toplevel_->base->initial_commit) {
    /* When an xdg_surface performs an initial commit, the compositor must
     * reply with a configure so the client can map the surface. tinywl
     * configures the xdg_toplevel with 0,0 size to let the client pick the
     * dimensions itself. */
    wlr_xdg_toplevel_set_size(wlr_xdg_toplevel_, 0, 0);
  }
}

void TinywlToplevel::xdg_toplevel_destroy(wlr::XdgToplevel::destroy::event_type const& UNUSED_ARG(event_type))
{
  delete this;   // FIXME: really?
}

void TinywlToplevel::begin_interactive(enum tinywl_cursor_mode mode, uint32_t edges) {
  /* This function sets up an interactive move or resize operation, where the
   * compositor stops propegating pointer events to clients and instead
   * consumes them itself, to move or resize windows. */
  TinywlServer* server = server_;

  server->grabbed_toplevel_ = this;
  server->cursor_mode_ = mode;

  if (mode == TINYWL_CURSOR_MOVE) {
    server->grab_x_ = server->wlr_cursor_->x - scene_tree_->node.x;
    server->grab_y_ = server->wlr_cursor_->y - scene_tree_->node.y;
  } else {
    struct wlr_box* geo_box = &wlr_xdg_toplevel_->base->geometry;

    double border_x = (scene_tree_->node.x + geo_box->x) +
      ((edges & WLR_EDGE_RIGHT) ? geo_box->width : 0);
    double border_y = (scene_tree_->node.y + geo_box->y) +
      ((edges & WLR_EDGE_BOTTOM) ? geo_box->height : 0);
    server->grab_x_ = server->wlr_cursor_->x - border_x;
    server->grab_y_ = server->wlr_cursor_->y - border_y;

    server->grab_geobox_ = *geo_box;
    server->grab_geobox_.x += scene_tree_->node.x;
    server->grab_geobox_.y += scene_tree_->node.y;

    server->resize_edges_ = edges;
  }
}

void TinywlToplevel::xdg_toplevel_request_move(wlr::XdgToplevel::request_move::event_type const& UNUSED_ARG(event_type))
{
  /* This event is raised when a client would like to begin an interactive
   * move, typically because the user clicked on their client-side
   * decorations. Note that a more sophisticated compositor should check the
   * provided serial against a list of button press serials sent to this
   * client, to prevent the client from requesting this whenever they want. */
  begin_interactive(TINYWL_CURSOR_MOVE, 0);
}

void TinywlToplevel::xdg_toplevel_request_resize(wlr::XdgToplevel::request_resize::event_type const& event)
{
  /* This event is raised when a client would like to begin an interactive
   * resize, typically because the user clicked on their client-side
   * decorations. Note that a more sophisticated compositor should check the
   * provided serial against a list of button press serials sent to this
   * client, to prevent the client from requesting this whenever they want. */
  begin_interactive(TINYWL_CURSOR_RESIZE, event->edges);
}

void TinywlToplevel::xdg_toplevel_request_maximize(wlr::XdgToplevel::request_maximize::event_type const& UNUSED_ARG(event_type))
{
  /* This event is raised when a client would like to maximize itself,
   * typically because the user clicked on the maximize button on client-side
   * decorations. tinywl doesn't support maximization, but to conform to
   * xdg-shell protocol we still must send a configure.
   * wlr_xdg_surface_schedule_configure() is used to send an empty reply.
   * However, if the request was sent before an initial commit, we don't do
   * anything and let the client finish the initial surface setup. */
  if (wlr_xdg_toplevel_->base->initialized)
    wlr_xdg_surface_schedule_configure(wlr_xdg_toplevel_->base);
}

void TinywlToplevel::xdg_toplevel_request_fullscreen(wlr::XdgToplevel::request_fullscreen::event_type const& UNUSED_ARG(event_type))
{
  /* Just as with request_maximize, we must send a configure here. */
  if (wlr_xdg_toplevel_->base->initialized)
    wlr_xdg_surface_schedule_configure(wlr_xdg_toplevel_->base);
}

/* This event is raised when a client creates a new toplevel (application window). */
void TinywlServer::xdg_shell_new_toplevel(wlr::XdgShell::new_toplevel::event_type const& xdg_toplevel)
{
  /* Allocate a TinywlToplevel for this surface */
  TinywlToplevel* toplevel = new TinywlToplevel(this, xdg_toplevel);
  toplevel->scene_tree_ =
          wlr_scene_xdg_surface_create(&toplevel->server_->scene_->tree, xdg_toplevel->base);
  toplevel->scene_tree_->node.data = toplevel;
  xdg_toplevel->base->data = toplevel->scene_tree_;
}

/* Called when a new surface state is committed. */
void TinywlPopup::surface_commit(wlr::Surface::commit::event_type const& data)
{
  if (wlr_xdg_popup_->base->initial_commit) {
    /* When an xdg_surface performs an initial commit, the compositor must
     * reply with a configure so the client can map the surface.
     * tinywl sends an empty configure. A more sophisticated compositor
     * might change an xdg_popup's geometry to ensure it's not positioned
     * off-screen, for example. */
    wlr_xdg_surface_schedule_configure(wlr_xdg_popup_->base);
  }
}

/* Called when the xdg_popup is destroyed. */
void TinywlPopup::xdg_popup_destroy(wlr::XdgPopup::destroy::event_type const& data)
{
  delete this;   // FIXME: really?
}

void TinywlServer::xdg_shell_new_popup(wlr::XdgShell::new_popup::event_type const& xdg_popup)
{
  /* This event is raised when a client creates a new popup. */
  TinywlPopup* popup = new TinywlPopup(xdg_popup);

  /* We must add xdg popups to the scene graph so they get rendered. The
   * wlroots scene graph provides a helper for this, but to use it we must
   * provide the proper parent scene node of the xdg popup. To enable this,
   * we always set the user data field of xdg_surfaces to the corresponding
   * scene node. */
  struct wlr_xdg_surface* parent = wlr_xdg_surface_try_from_wlr_surface(xdg_popup->parent);
  assert(parent != NULL);
  struct wlr_scene_tree *parent_tree = (struct wlr_scene_tree *)parent->data;
  xdg_popup->base->data = wlr_scene_xdg_surface_create(parent_tree, xdg_popup->base);
}

int main(int argc, char *argv[])
{
  Debug(NAMESPACE_DEBUG::init());

  wlr_log_init(WLR_DEBUG, NULL);
  char const* startup_cmd = nullptr;

  int c;
  while ((c = getopt(argc, argv, "s:h")) != -1)
  {
    switch (c)
    {
      case 's':
        startup_cmd = optarg;
        break;
      default:
        printf("Usage: %s [-s startup command]\n", argv[0]);
        return 0;
    }
  }
  if (optind < argc)
  {
    printf("Usage: %s [-s startup command]\n", argv[0]);
    return 0;
  }

  try
  {
    TinywlServer server(startup_cmd);

    // Run the Wayland event loop. This does not return until you exit the
    // compositor. Starting the backend rigged up all of the necessary event
    // loop configuration to listen to libinput events, DRM events, generate
    // frame events at the refresh rate, and so on.
    server.run();

    // Once wl_display_run returns, server is destructed which will destroy all clients and then shut down the server.
  }
  catch (AIAlert::Error const& error)
  {
#ifdef CWDEBUG
    std::stringstream oss;
    oss << error;
    wlr_log(WLR_ERROR, "%s", oss.str().c_str());
#endif
  }
}
