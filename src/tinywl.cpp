#include "sys.h"

#include <cassert>
#include <cstdbool>
#include <cstdlib>
#include <ctime>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <wayland-server-core.h>

// wlroots doesn't support inclusion from C++?!
extern "C" {
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
//#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>
} // extern "C"

#include <xkbcommon/xkbcommon.h>

#include "wlr/listener.h"
#include "wlr/events.h"
#include "wlr/Scene.h"
#include "wl/Listener.h"
#include "debug.h"

#undef wl_container_of
#define wl_container_of(...) nullptr

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

/* For brevity's sake, struct members are annotated where they are used. */
enum tinywl_cursor_mode {
	TINYWL_CURSOR_PASSTHROUGH,
	TINYWL_CURSOR_MOVE,
	TINYWL_CURSOR_RESIZE,
};

struct tinywl_server
{
  struct wl_display *wl_display;

  struct    wlr_backend*                                backend;
  listener::wlr_backend::new_input                      backend_new_input_;
  listener::wlr_backend::new_input::handle_t            backend_new_input_handle_;
  listener::wlr_backend::new_output                     backend_new_output_;
  listener::wlr_backend::new_output::handle_t           backend_new_output_handle_;

  struct wlr_renderer *renderer;
  struct wlr_allocator *allocator;
  wlr::Scene scene_;
  struct wlr_scene_output_layout *scene_layout;

  struct    wlr_xdg_shell*                              xdg_shell;
  listener::wlr_xdg_shell::new_toplevel                 xdg_shell_new_toplevel_;
  listener::wlr_xdg_shell::new_toplevel::handle_t       xdg_shell_new_toplevel_handle_;
  listener::wlr_xdg_shell::new_popup                    xdg_shell_new_popup_;
  listener::wlr_xdg_shell::new_popup::handle_t          xdg_shell_new_popup_handle_;

  struct wl_list toplevels;

  struct    wlr_cursor*                                 cursor;
  listener::wlr_cursor::motion                          cursor_motion_;
  listener::wlr_cursor::motion::handle_t                cursor_motion_handle_;
  listener::wlr_cursor::motion_absolute                 cursor_motion_absolute_;
  listener::wlr_cursor::motion_absolute::handle_t       cursor_motion_absolute_handle_;
  listener::wlr_cursor::button                          cursor_button_;
  listener::wlr_cursor::button::handle_t                cursor_button_handle_;
  listener::wlr_cursor::axis                            cursor_axis_;
  listener::wlr_cursor::axis::handle_t                  cursor_axis_handle_;
  listener::wlr_cursor::frame                           cursor_frame_;
  listener::wlr_cursor::frame::handle_t                 cursor_frame_handle_;

  struct wlr_xcursor_manager *cursor_mgr;

  struct    wlr_seat*                                   seat;
  listener::wlr_seat::request_set_cursor                seat_request_set_cursor_;
  listener::wlr_seat::request_set_cursor::handle_t      seat_request_set_cursor_handle_;
  listener::wlr_seat::request_set_selection             seat_request_set_selection_;
  listener::wlr_seat::request_set_selection::handle_t   seat_request_set_selection_handle_;

  struct wl_list keyboards;
  enum tinywl_cursor_mode cursor_mode;
  struct tinywl_toplevel *grabbed_toplevel;
  double grab_x, grab_y;
  struct wlr_box grab_geobox;
  uint32_t resize_edges;

  struct wlr_output_layout *output_layout;
  struct wl_list outputs;

  // Callback for xdg_shell_new_toplevel_.
  void xdg_shell_new_toplevel(listener::wlr_xdg_shell::new_toplevel::event_type_t const& xdg_toplevel);
  // Callback for backend_new_input_.
  void backend_new_input(listener::wlr_backend::new_input::event_type_t const& device);
  // Callback for xdg_shell_new_popup_.
  void xdg_shell_new_popup(listener::wlr_xdg_shell::new_popup::event_type_t const& xdg_popup);
  // Callback for cursor_motion_.
  void cursor_motion(listener::wlr_cursor::motion::event_type_t const& event);
  // Callback for cursor_motion_absolute_.
  void cursor_motion_absolute(listener::wlr_cursor::motion_absolute::event_type_t const& event);
  // Callback for cursor_button_.
  void cursor_button(listener::wlr_cursor::button::event_type_t const& event);
  // Callback for cursor_axis_.
  void cursor_axis(listener::wlr_cursor::axis::event_type_t const& event);
  // Callback for cursor_frame_.
  void cursor_frame(listener::wlr_cursor::frame::event_type_t const& data);
  // Callback for seat_request_set_cursor_.
  void seat_request_set_cursor(listener::wlr_seat::request_set_cursor::event_type_t const& event);
  // Callback for seat_request_set_selection_.
  void seat_request_set_selection(listener::wlr_seat::request_set_selection::event_type_t const& event);
  // Callback for backend_new_output_.
  void new_output(listener::wlr_backend::new_output::event_type_t const& output);
};

struct tinywl_output
{
  struct wl_list link;
  struct tinywl_server *server;

  struct    wlr_output*                                 wlr_output_;
  listener::wlr_output::frame                           output_frame_;
  listener::wlr_output::frame::handle_t                 output_frame_handle_;
  listener::wlr_output::request_state                   output_request_state_;
  listener::wlr_output::request_state::handle_t         output_request_state_handle_;
  listener::wlr_output::destroy                         output_destroy_;
  listener::wlr_output::destroy::handle_t               output_destroy_handle_;

  // Callback for output_frame_.
  void output_frame(listener::wlr_output::frame::event_type_t const& data);
  // Callback for output_request_state_.
  void output_request_state(listener::wlr_output::request_state::event_type_t const& data);
  // Callback for output_destroy_.
  void output_destroy(listener::wlr_output::destroy::event_type_t const& data);
};

struct tinywl_toplevel
{
  struct wl_list link;
  struct tinywl_server *server;
  struct wlr_xdg_toplevel *xdg_toplevel;
//	struct wlr_scene_tree *scene_tree;
  struct wl_listener map;
  struct wl_listener unmap;
  struct wl_listener commit;
  struct wl_listener destroy;
  struct wl_listener request_move;
  struct wl_listener request_resize;
  struct wl_listener request_maximize;
  struct wl_listener request_fullscreen;
};

struct tinywl_popup
{
  struct wlr_xdg_popup *xdg_popup;
  struct wl_listener commit;
  struct wl_listener destroy;
};

struct tinywl_keyboard
{
  struct wl_list link;
  struct tinywl_server *server;
  struct wlr_keyboard *wlr_keyboard;

  struct wl_listener modifiers;
  struct wl_listener key;
  struct wl_listener destroy;
};

static void focus_toplevel(struct tinywl_toplevel *toplevel) {
	/* Note: this function only deals with keyboard focus. */
	if (toplevel == NULL) {
		return;
	}
	struct tinywl_server *server = toplevel->server;
	struct wlr_seat *seat = server->seat;
	struct wlr_surface *prev_surface = seat->keyboard_state.focused_surface;
	struct wlr_surface *surface = toplevel->xdg_toplevel->base->surface;
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
		struct wlr_xdg_toplevel *prev_toplevel =
			wlr_xdg_toplevel_try_from_wlr_surface(prev_surface);
		if (prev_toplevel != NULL) {
			wlr_xdg_toplevel_set_activated(prev_toplevel, false);
		}
	}
	struct wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat);
	/* Move the toplevel to the front */
#if 0 // FIXME: we don't have `toplevel->scene_tree`
	wlr_scene_node_raise_to_top(&toplevel->scene_tree->node);
#endif
	wl_list_remove(&toplevel->link);
	wl_list_insert(&server->toplevels, &toplevel->link);
	/* Activate the new surface */
	wlr_xdg_toplevel_set_activated(toplevel->xdg_toplevel, true);
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

static void keyboard_handle_modifiers(
		struct wl_listener *listener, void *data) {
	/* This event is raised when a modifier key, such as shift or alt, is
	 * pressed. We simply communicate this to the client. */
	struct tinywl_keyboard *keyboard =
		wl_container_of(listener, keyboard, modifiers);
	/*
	 * A seat can only have one keyboard, but this is a limitation of the
	 * Wayland protocol - not wlroots. We assign all connected keyboards to the
	 * same seat. You can swap out the underlying wlr_keyboard like this and
	 * wlr_seat handles this transparently.
	 */
	wlr_seat_set_keyboard(keyboard->server->seat, keyboard->wlr_keyboard);
	/* Send modifiers to the client. */
	wlr_seat_keyboard_notify_modifiers(keyboard->server->seat,
		&keyboard->wlr_keyboard->modifiers);
}

static bool handle_keybinding(struct tinywl_server *server, xkb_keysym_t sym) {
	/*
	 * Here we handle compositor keybindings. This is when the compositor is
	 * processing keys, rather than passing them on to the client for its own
	 * processing.
	 *
	 * This function assumes Alt is held down.
	 */
	switch (sym) {
	case XKB_KEY_Escape:
		wl_display_terminate(server->wl_display);
		break;
	case XKB_KEY_F1:
              {
		/* Cycle to the next toplevel */
		if (wl_list_length(&server->toplevels) < 2) {
			break;
		}
		struct tinywl_toplevel *next_toplevel =
			wl_container_of(server->toplevels.prev, next_toplevel, link);
		focus_toplevel(next_toplevel);
		break;
              }
	default:
		return false;
	}
	return true;
}

static void keyboard_handle_key(
		struct wl_listener *listener, void *data) {
	/* This event is raised when a key is pressed or released. */
	struct tinywl_keyboard *keyboard =
		wl_container_of(listener, keyboard, key);
	struct tinywl_server *server = keyboard->server;
	struct wlr_keyboard_key_event *event = (struct wlr_keyboard_key_event *)(data);
	struct wlr_seat *seat = server->seat;

	/* Translate libinput keycode -> xkbcommon */
	uint32_t keycode = event->keycode + 8;
	/* Get a list of keysyms based on the keymap for this keyboard */
	const xkb_keysym_t *syms;
	int nsyms = xkb_state_key_get_syms(
			keyboard->wlr_keyboard->xkb_state, keycode, &syms);

	bool handled = false;
	uint32_t modifiers = wlr_keyboard_get_modifiers(keyboard->wlr_keyboard);
	if ((modifiers & WLR_MODIFIER_ALT) &&
			event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
		/* If alt is held down and this button was _pressed_, we attempt to
		 * process it as a compositor keybinding. */
		for (int i = 0; i < nsyms; i++) {
			handled = handle_keybinding(server, syms[i]);
		}
	}

	if (!handled) {
		/* Otherwise, we pass it along to the client. */
		wlr_seat_set_keyboard(seat, keyboard->wlr_keyboard);
		wlr_seat_keyboard_notify_key(seat, event->time_msec,
			event->keycode, event->state);
	}
}

static void keyboard_handle_destroy(struct wl_listener *listener, void *data) {
	/* This event is raised by the keyboard base wlr_input_device to signal
	 * the destruction of the wlr_keyboard. It will no longer receive events
	 * and should be destroyed.
	 */
	struct tinywl_keyboard *keyboard =
		wl_container_of(listener, keyboard, destroy);
	wl_list_remove(&keyboard->modifiers.link);
	wl_list_remove(&keyboard->key.link);
	wl_list_remove(&keyboard->destroy.link);
	wl_list_remove(&keyboard->link);
	free(keyboard);
}

static void server_new_keyboard(struct tinywl_server *server,
		struct wlr_input_device *device) {
	struct wlr_keyboard *wlr_keyboard = wlr_keyboard_from_input_device(device);

	struct tinywl_keyboard *keyboard = (struct tinywl_keyboard *)calloc(1, sizeof(*keyboard));
	keyboard->server = server;
	keyboard->wlr_keyboard = wlr_keyboard;

	/* We need to prepare an XKB keymap and assign it to the keyboard. This
	 * assumes the defaults (e.g. layout = "us"). */
	struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	struct xkb_keymap *keymap = xkb_keymap_new_from_names(context, NULL,
		XKB_KEYMAP_COMPILE_NO_FLAGS);

	wlr_keyboard_set_keymap(wlr_keyboard, keymap);
	xkb_keymap_unref(keymap);
	xkb_context_unref(context);
	wlr_keyboard_set_repeat_info(wlr_keyboard, 25, 600);

	/* Here we set up listeners for keyboard events. */
	keyboard->modifiers.notify = keyboard_handle_modifiers;
	wl_signal_add(&wlr_keyboard->events.modifiers, &keyboard->modifiers);
	keyboard->key.notify = keyboard_handle_key;
	wl_signal_add(&wlr_keyboard->events.key, &keyboard->key);
	keyboard->destroy.notify = keyboard_handle_destroy;
	wl_signal_add(&device->events.destroy, &keyboard->destroy);

	wlr_seat_set_keyboard(server->seat, keyboard->wlr_keyboard);

	/* And add the keyboard to our list of keyboards */
	wl_list_insert(&server->keyboards, &keyboard->link);
}

static void server_new_pointer(struct tinywl_server *server,
		struct wlr_input_device *device) {
	/* We don't do anything special with pointers. All of our pointer handling
	 * is proxied through wlr_cursor. On another compositor, you might take this
	 * opportunity to do libinput configuration on the device to set
	 * acceleration, etc. */
	wlr_cursor_attach_input_device(server->cursor, device);
}

void tinywl_server::backend_new_input(listener::wlr_backend::new_input::event_type_t const& device)
{
  /* This event is raised by the backend when a new input device becomes available. */
  switch (device->type)
  {
    case WLR_INPUT_DEVICE_KEYBOARD:
      server_new_keyboard(this, device);
      break;
    case WLR_INPUT_DEVICE_POINTER:
      server_new_pointer(this, device);
      break;
    default:
      break;
  }
  /* We need to let the wlr_seat know what our capabilities are, which is
   * communiciated to the client. In TinyWL we always have a cursor, even if
   * there are no pointer devices, so we always include that capability. */
  uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
  if (!wl_list_empty(&keyboards))
    caps |= WL_SEAT_CAPABILITY_KEYBOARD;
  wlr_seat_set_capabilities(seat, caps);
}

void tinywl_server::seat_request_set_cursor(listener::wlr_seat::request_set_cursor::event_type_t const& event)
{
  /* This event is raised by the seat when a client provides a cursor image */
  struct wlr_seat_client *focused_client = seat->pointer_state.focused_client;
  /* This can be sent by any client, so we check to make sure this one is
   * actually has pointer focus first. */
  if (focused_client == event->seat_client) {
    /* Once we've vetted the client, we can tell the cursor to use the
     * provided surface as the cursor image. It will set the hardware cursor
     * on the output that it's currently on and continue to do so as the
     * cursor moves between outputs. */
    wlr_cursor_set_surface(cursor, event->surface,
        event->hotspot_x, event->hotspot_y);
  }
}

void tinywl_server::seat_request_set_selection(listener::wlr_seat::request_set_selection::event_type_t const& event)
{
  /* This event is raised by the seat when a client wants to set the selection,
   * usually when the user copies something. wlroots allows compositors to
   * ignore such requests if they so choose, but in tinywl we always honor
   */
  wlr_seat_set_selection(seat, event->source, event->serial);
}

static struct tinywl_toplevel *desktop_toplevel_at(
		struct tinywl_server *server, double lx, double ly,
		struct wlr_surface **surface, double *sx, double *sy) {
	/* This returns the topmost node in the scene at the given layout coords.
	 * We only care about surface nodes as we are specifically looking for a
	 * surface in the surface tree of a tinywl_toplevel. */
#if 0   // FIXME: we don't have `server->scene`
	struct wlr_scene_node *node = wlr_scene_node_at(&server->scene->tree.node, lx, ly, sx, sy);
	if (node == NULL || node->type != WLR_SCENE_NODE_BUFFER) {
		return NULL;
	}
	struct wlr_scene_buffer *scene_buffer = wlr_scene_buffer_from_node(node);
	struct wlr_scene_surface *scene_surface =
		wlr_scene_surface_try_from_buffer(scene_buffer);
	if (!scene_surface) {
		return NULL;
	}

	*surface = scene_surface->surface;
	/* Find the node corresponding to the tinywl_toplevel at the root of this
	 * surface tree, it is the only one for which we set the data field. */
	struct wlr_scene_tree *tree = node->parent;
	while (tree != NULL && tree->node.data == NULL) {
		tree = tree->node.parent;
	}
	return tree->node.data;
#else
        return NULL;
#endif
}

static void reset_cursor_mode(struct tinywl_server *server) {
	/* Reset the cursor mode to passthrough. */
	server->cursor_mode = TINYWL_CURSOR_PASSTHROUGH;
	server->grabbed_toplevel = NULL;
}

static void process_cursor_move(struct tinywl_server *server) {
	/* Move the grabbed toplevel to the new position. */
	struct tinywl_toplevel *toplevel = server->grabbed_toplevel;
#if 0 // FIXME: we don't have `toplevel->scene_tree`
	wlr_scene_node_set_position(&toplevel->scene_tree->node,
		server->cursor->x - server->grab_x,
		server->cursor->y - server->grab_y);
#endif
}

static void process_cursor_resize(struct tinywl_server *server) {
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
	struct tinywl_toplevel *toplevel = server->grabbed_toplevel;
	double border_x = server->cursor->x - server->grab_x;
	double border_y = server->cursor->y - server->grab_y;
	int new_left = server->grab_geobox.x;
	int new_right = server->grab_geobox.x + server->grab_geobox.width;
	int new_top = server->grab_geobox.y;
	int new_bottom = server->grab_geobox.y + server->grab_geobox.height;

	if (server->resize_edges & WLR_EDGE_TOP) {
		new_top = border_y;
		if (new_top >= new_bottom) {
			new_top = new_bottom - 1;
		}
	} else if (server->resize_edges & WLR_EDGE_BOTTOM) {
		new_bottom = border_y;
		if (new_bottom <= new_top) {
			new_bottom = new_top + 1;
		}
	}
	if (server->resize_edges & WLR_EDGE_LEFT) {
		new_left = border_x;
		if (new_left >= new_right) {
			new_left = new_right - 1;
		}
	} else if (server->resize_edges & WLR_EDGE_RIGHT) {
		new_right = border_x;
		if (new_right <= new_left) {
			new_right = new_left + 1;
		}
	}

	struct wlr_box *geo_box = &toplevel->xdg_toplevel->base->geometry;
#if 0 // FIXME: we don't have `toplevel->scene_tree`
	wlr_scene_node_set_position(&toplevel->scene_tree->node,
		new_left - geo_box->x, new_top - geo_box->y);
#endif

	int new_width = new_right - new_left;
	int new_height = new_bottom - new_top;
	wlr_xdg_toplevel_set_size(toplevel->xdg_toplevel, new_width, new_height);
}

static void process_cursor_motion(struct tinywl_server *server, uint32_t time) {
	/* If the mode is non-passthrough, delegate to those functions. */
	if (server->cursor_mode == TINYWL_CURSOR_MOVE) {
		process_cursor_move(server);
		return;
	} else if (server->cursor_mode == TINYWL_CURSOR_RESIZE) {
		process_cursor_resize(server);
		return;
	}

	/* Otherwise, find the toplevel under the pointer and send the event along. */
	double sx, sy;
	struct wlr_seat *seat = server->seat;
	struct wlr_surface *surface = NULL;
	struct tinywl_toplevel *toplevel = desktop_toplevel_at(server,
			server->cursor->x, server->cursor->y, &surface, &sx, &sy);
	if (!toplevel) {
		/* If there's no toplevel under the cursor, set the cursor image to a
		 * default. This is what makes the cursor image appear when you move it
		 * around the screen, not over any toplevels. */
		wlr_cursor_set_xcursor(server->cursor, server->cursor_mgr, "default");
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

void tinywl_server::cursor_motion(listener::wlr_cursor::motion::event_type_t const& event)
{
  /* This event is forwarded by the cursor when a pointer emits a _relative_
   * pointer motion event (i.e. a delta) */
  /* The cursor doesn't move unless we tell it to. The cursor automatically
   * handles constraining the motion to the output layout, as well as any
   * special configuration applied for the specific input device which
   * generated the event. You can pass NULL for the device if you want to move
   * the cursor around without any input. */
  wlr_cursor_move(cursor, &event->pointer->base, event->delta_x, event->delta_y);
  process_cursor_motion(this, event->time_msec);
}

void tinywl_server::cursor_motion_absolute(listener::wlr_cursor::motion_absolute::event_type_t const& event)
{
  /* This event is forwarded by the cursor when a pointer emits an _absolute_
   * motion event, from 0..1 on each axis. This happens, for example, when
   * wlroots is running under a Wayland window rather than KMS+DRM, and you
   * move the mouse over the window. You could enter the window from any edge,
   * so we have to warp the mouse there. There is also some hardware which
   * emits these events. */
  wlr_cursor_warp_absolute(cursor, &event->pointer->base, event->x, event->y);
  process_cursor_motion(this, event->time_msec);
}

void tinywl_server::cursor_button(listener::wlr_cursor::button::event_type_t const& event)
{
  /* This event is forwarded by the cursor when a pointer emits a button
   * event. */
  /* Notify the client with pointer focus that a button press has occurred */
  wlr_seat_pointer_notify_button(seat,
      event->time_msec, event->button, event->state);
  if (event->state == WL_POINTER_BUTTON_STATE_RELEASED) {
    /* If you released any buttons, we exit interactive move/resize mode. */
    reset_cursor_mode(this);
  } else {
    /* Focus that client if the button was _pressed_ */
    double sx, sy;
    struct wlr_surface *surface = NULL;
    struct tinywl_toplevel *toplevel = desktop_toplevel_at(this,
        cursor->x, cursor->y, &surface, &sx, &sy);
    focus_toplevel(toplevel);
  }
}

void tinywl_server::cursor_axis(listener::wlr_cursor::axis::event_type_t const& event)
{
  /* This event is forwarded by the cursor when a pointer emits an axis event,
   * for example when you move the scroll wheel. */
  /* Notify the client with pointer focus of the axis event. */
  wlr_seat_pointer_notify_axis(seat,
      event->time_msec, event->orientation, event->delta,
      event->delta_discrete, event->source, event->relative_direction);
}

void tinywl_server::cursor_frame(listener::wlr_cursor::frame::event_type_t const& data)
{
  /* This event is forwarded by the cursor when a pointer emits an frame
   * event. Frame events are sent after regular pointer events to group
   * multiple events together. For instance, two axis events may happen at the
   * same time, in which case a frame event won't be sent in between. */
  /* Notify the client with pointer focus of the frame event. */
  wlr_seat_pointer_notify_frame(seat);
}

void tinywl_output::output_frame(listener::wlr_output::frame::event_type_t const&)
{
  /* This function is called every time an output is ready to display a frame,
   * generally at the output's refresh rate (e.g. 60Hz). */
#if 0 // FIXME: we don't have `server->scene`.
  struct wlr_scene *scene = server->scene;

  struct wlr_scene_output *scene_output = wlr_scene_get_scene_output(scene, wlr_output_);

  /* Render the scene if needed and commit the output */
  wlr_scene_output_commit(scene_output, NULL);

  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  wlr_scene_output_send_frame_done(scene_output, &now);
#endif
}

void tinywl_output::output_request_state(listener::wlr_output::request_state::event_type_t const& event)
{
  /* This function is called when the backend requests a new state for
   * the output. For example, Wayland and X11 backends request a new mode
   * when the output window is resized. */
  wlr_output_commit_state(wlr_output_, event->state);
}

void tinywl_output::output_destroy(listener::wlr_output::destroy::event_type_t const&)
{
  output_frame_handle_.cancel();
  output_request_state_handle_.cancel();
  output_destroy_handle_.cancel();
  wl_list_remove(&link);
}

void tinywl_server::new_output(listener::wlr_backend::new_output::event_type_t const& wlr_output)
{
  /* This event is raised by the backend when a new output (aka a display or
   * monitor) becomes available. */

  /* Configures the output created by the backend to use our allocator
   * and our renderer. Must be done once, before commiting the output */
  wlr_output_init_render(wlr_output, allocator, renderer);

  /* The output may be disabled, switch it on. */
  struct wlr_output_state state;
  wlr_output_state_init(&state);
  wlr_output_state_set_enabled(&state, true);

  /* Some backends don't have modes. DRM+KMS does, and we need to set a mode
   * before we can use the output. The mode is a tuple of (width, height,
   * refresh rate), and each monitor supports only a specific set of modes. We
   * just pick the monitor's preferred mode, a more sophisticated compositor
   * would let the user configure it. */
  struct wlr_output_mode *mode = wlr_output_preferred_mode(wlr_output);
  if (mode != NULL) {
    wlr_output_state_set_mode(&state, mode);
  }

  /* Atomically applies the new output state. */
  wlr_output_commit_state(wlr_output, &state);
  wlr_output_state_finish(&state);

  /* Allocates and configures our state for this output */
  struct tinywl_output *output = (struct tinywl_output *)calloc(1, sizeof(*output));
  output->wlr_output_ = wlr_output;
  output->server = this;

  // Initialize the output_frame_ listener.
  output->output_frame_.init(&wlr_output->events.frame);
  output->output_frame_handle_ = output->output_frame_.request(*output, &tinywl_output::output_frame);

  // Initialize the output_request_state_ listener.
  output->output_request_state_.init(&wlr_output->events.request_state);
  output->output_request_state_handle_ = output->output_request_state_.request(*output, &tinywl_output::output_request_state);

  // Initialize the output_destroy_ listener.
  output->output_destroy_.init(&wlr_output->events.destroy);
  output->output_destroy_handle_ = output->output_destroy_.request(*output, &tinywl_output::output_destroy);

  wl_list_insert(&outputs, &output->link);

  /* Adds this to the output layout. The add_auto function arranges outputs
   * from left-to-right in the order they appear. A more sophisticated
   * compositor would let the user configure the arrangement of outputs in the
   * layout.
   *
   * The output layout utility automatically adds a wl_output global to the
   * display, which Wayland clients can see to find out information about the
   * output (such as DPI, scale factor, manufacturer, etc).
   */
  struct wlr_output_layout_output *l_output = wlr_output_layout_add_auto(output_layout,
    wlr_output);
#if 0 // FIXME: we don't have server->scene.
  struct wlr_scene_output *scene_output = wlr_scene_output_create(server->scene, wlr_output);
  wlr_scene_output_layout_add_output(server->scene_layout, l_output, scene_output);
#endif
}

/* The static functions were replaced by member functions above */

static void xdg_toplevel_map(struct wl_listener *listener, void *data) {
  /* Called when the surface is mapped, or ready to display on-screen. */
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, map);

  wl_list_insert(&toplevel->server->toplevels, &toplevel->link);

  focus_toplevel(toplevel);
}

static void xdg_toplevel_unmap(struct wl_listener *listener, void *data) {
  /* Called when the surface is unmapped, and should no longer be shown. */
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, unmap);

  /* Reset the cursor mode if the grabbed toplevel was unmapped. */
  if (toplevel == toplevel->server->grabbed_toplevel) {
    reset_cursor_mode(toplevel->server);
  }

  wl_list_remove(&toplevel->link);
}

static void xdg_toplevel_commit(struct wl_listener *listener, void *data) {
  /* Called when a new surface state is committed. */
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, commit);

  if (toplevel->xdg_toplevel->base->initial_commit) {
    /* When an xdg_surface performs an initial commit, the compositor must
     * reply with a configure so the client can map the surface. tinywl
     * configures the xdg_toplevel with 0,0 size to let the client pick the
     * dimensions itself. */
    wlr_xdg_toplevel_set_size(toplevel->xdg_toplevel, 0, 0);
  }
}

static void xdg_toplevel_destroy(struct wl_listener *listener, void *data) {
  /* Called when the xdg_toplevel is destroyed. */
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, destroy);

  wl_list_remove(&toplevel->map.link);
  wl_list_remove(&toplevel->unmap.link);
  wl_list_remove(&toplevel->commit.link);
  wl_list_remove(&toplevel->destroy.link);
  wl_list_remove(&toplevel->request_move.link);
  wl_list_remove(&toplevel->request_resize.link);
  wl_list_remove(&toplevel->request_maximize.link);
  wl_list_remove(&toplevel->request_fullscreen.link);

  free(toplevel);
}

static void begin_interactive(struct tinywl_toplevel *toplevel,
    enum tinywl_cursor_mode mode, uint32_t edges) {
  /* This function sets up an interactive move or resize operation, where the
   * compositor stops propegating pointer events to clients and instead
   * consumes them itself, to move or resize windows. */
  struct tinywl_server *server = toplevel->server;

  server->grabbed_toplevel = toplevel;
  server->cursor_mode = mode;

#if 0 // FIXME: we don't have `toplevel->scene_tree`.
  if (mode == TINYWL_CURSOR_MOVE) {
    server->grab_x = server->cursor->x - toplevel->scene_tree->node.x;
    server->grab_y = server->cursor->y - toplevel->scene_tree->node.y;
  } else {
    struct wlr_box *geo_box = &toplevel->xdg_toplevel->base->geometry;

    double border_x = (toplevel->scene_tree->node.x + geo_box->x) +
      ((edges & WLR_EDGE_RIGHT) ? geo_box->width : 0);
    double border_y = (toplevel->scene_tree->node.y + geo_box->y) +
      ((edges & WLR_EDGE_BOTTOM) ? geo_box->height : 0);
    server->grab_x = server->cursor->x - border_x;
    server->grab_y = server->cursor->y - border_y;

    server->grab_geobox = *geo_box;
    server->grab_geobox.x += toplevel->scene_tree->node.x;
    server->grab_geobox.y += toplevel->scene_tree->node.y;

    server->resize_edges = edges;
  }
#endif
}

static void xdg_toplevel_request_move(
    struct wl_listener *listener, void *data) {
  /* This event is raised when a client would like to begin an interactive
   * move, typically because the user clicked on their client-side
   * decorations. Note that a more sophisticated compositor should check the
   * provided serial against a list of button press serials sent to this
   * client, to prevent the client from requesting this whenever they want. */
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, request_move);
  begin_interactive(toplevel, TINYWL_CURSOR_MOVE, 0);
}

static void xdg_toplevel_request_resize(
    struct wl_listener *listener, void *data) {
  /* This event is raised when a client would like to begin an interactive
   * resize, typically because the user clicked on their client-side
   * decorations. Note that a more sophisticated compositor should check the
   * provided serial against a list of button press serials sent to this
   * client, to prevent the client from requesting this whenever they want. */
  struct wlr_xdg_toplevel_resize_event *event = (struct wlr_xdg_toplevel_resize_event *)data;
  struct tinywl_toplevel *toplevel = wl_container_of(listener, toplevel, request_resize);
  begin_interactive(toplevel, TINYWL_CURSOR_RESIZE, event->edges);
}

static void xdg_toplevel_request_maximize(
    struct wl_listener *listener, void *data) {
  /* This event is raised when a client would like to maximize itself,
   * typically because the user clicked on the maximize button on client-side
   * decorations. tinywl doesn't support maximization, but to conform to
   * xdg-shell protocol we still must send a configure.
   * wlr_xdg_surface_schedule_configure() is used to send an empty reply.
   * However, if the request was sent before an initial commit, we don't do
   * anything and let the client finish the initial surface setup. */
  struct tinywl_toplevel *toplevel =
    wl_container_of(listener, toplevel, request_maximize);
  if (toplevel->xdg_toplevel->base->initialized) {
    wlr_xdg_surface_schedule_configure(toplevel->xdg_toplevel->base);
  }
}

static void xdg_toplevel_request_fullscreen(
    struct wl_listener *listener, void *data) {
  /* Just as with request_maximize, we must send a configure here. */
  struct tinywl_toplevel *toplevel =
    wl_container_of(listener, toplevel, request_fullscreen);
  if (toplevel->xdg_toplevel->base->initialized) {
    wlr_xdg_surface_schedule_configure(toplevel->xdg_toplevel->base);
  }
}

/* This event is raised when a client creates a new toplevel (application window). */
void tinywl_server::xdg_shell_new_toplevel(listener::wlr_xdg_shell::new_toplevel::event_type_t const& xdg_toplevel)
{
  /* Allocate a tinywl_toplevel for this surface */
  struct tinywl_toplevel *toplevel = (struct tinywl_toplevel *)calloc(1, sizeof(*toplevel));
  toplevel->server = this;
  toplevel->xdg_toplevel = xdg_toplevel;
#if 0 // FIXME
  toplevel->scene_tree =
          wlr_scene_xdg_surface_create(&toplevel->server->scene->tree, xdg_toplevel->base);
  toplevel->scene_tree->node.data = toplevel;
  xdg_toplevel->base->data = toplevel->scene_tree;
#endif

  /* Listen to the various events it can emit */
  toplevel->map.notify = xdg_toplevel_map;
  wl_signal_add(&xdg_toplevel->base->surface->events.map, &toplevel->map);
  toplevel->unmap.notify = xdg_toplevel_unmap;
  wl_signal_add(&xdg_toplevel->base->surface->events.unmap, &toplevel->unmap);
  toplevel->commit.notify = xdg_toplevel_commit;
  wl_signal_add(&xdg_toplevel->base->surface->events.commit, &toplevel->commit);

  toplevel->destroy.notify = xdg_toplevel_destroy;
  wl_signal_add(&xdg_toplevel->events.destroy, &toplevel->destroy);

  /* cotd */
  toplevel->request_move.notify = xdg_toplevel_request_move;
  wl_signal_add(&xdg_toplevel->events.request_move, &toplevel->request_move);
  toplevel->request_resize.notify = xdg_toplevel_request_resize;
  wl_signal_add(&xdg_toplevel->events.request_resize, &toplevel->request_resize);
  toplevel->request_maximize.notify = xdg_toplevel_request_maximize;
  wl_signal_add(&xdg_toplevel->events.request_maximize, &toplevel->request_maximize);
  toplevel->request_fullscreen.notify = xdg_toplevel_request_fullscreen;
  wl_signal_add(&xdg_toplevel->events.request_fullscreen, &toplevel->request_fullscreen);
}

static void xdg_popup_commit(struct wl_listener *listener, void *data) {
  /* Called when a new surface state is committed. */
  struct tinywl_popup *popup = wl_container_of(listener, popup, commit);

  if (popup->xdg_popup->base->initial_commit) {
    /* When an xdg_surface performs an initial commit, the compositor must
     * reply with a configure so the client can map the surface.
     * tinywl sends an empty configure. A more sophisticated compositor
     * might change an xdg_popup's geometry to ensure it's not positioned
     * off-screen, for example. */
    wlr_xdg_surface_schedule_configure(popup->xdg_popup->base);
  }
}

static void xdg_popup_destroy(struct wl_listener *listener, void *data) {
  /* Called when the xdg_popup is destroyed. */
  struct tinywl_popup *popup = wl_container_of(listener, popup, destroy);

  wl_list_remove(&popup->commit.link);
  wl_list_remove(&popup->destroy.link);

  free(popup);
}

void tinywl_server::xdg_shell_new_popup(listener::wlr_xdg_shell::new_popup::event_type_t const& xdg_popup)
{
  /* This event is raised when a client creates a new popup. */
  struct tinywl_popup *popup = (struct tinywl_popup *)calloc(1, sizeof(*popup));
  popup->xdg_popup = xdg_popup;

  /* We must add xdg popups to the scene graph so they get rendered. The
   * wlroots scene graph provides a helper for this, but to use it we must
   * provide the proper parent scene node of the xdg popup. To enable this,
   * we always set the user data field of xdg_surfaces to the corresponding
   * scene node. */
  struct wlr_xdg_surface *parent = wlr_xdg_surface_try_from_wlr_surface(xdg_popup->parent);
  assert(parent != NULL);
#if 0 // FIXME
  struct wlr_scene_tree *parent_tree = (struct wlr_scene_tree *)parent->data;
  xdg_popup->base->data = wlr_scene_xdg_surface_create(parent_tree, xdg_popup->base);
#endif

  popup->commit.notify = xdg_popup_commit;
  wl_signal_add(&xdg_popup->base->surface->events.commit, &popup->commit);

  popup->destroy.notify = xdg_popup_destroy;
  wl_signal_add(&xdg_popup->events.destroy, &popup->destroy);
}

int main(int argc, char *argv[]) {
  Debug(NAMESPACE_DEBUG::init());

  wlr_log_init(WLR_DEBUG, NULL);
  char *startup_cmd = NULL;

  int c;
  while ((c = getopt(argc, argv, "s:h")) != -1) {
    switch (c) {
    case 's':
      startup_cmd = optarg;
      break;
    default:
      printf("Usage: %s [-s startup command]\n", argv[0]);
      return 0;
    }
  }
  if (optind < argc) {
    printf("Usage: %s [-s startup command]\n", argv[0]);
    return 0;
  }

  struct tinywl_server server = {0};
  /* The Wayland display is managed by libwayland. It handles accepting
   * clients from the Unix socket, manging Wayland globals, and so on. */
  server.wl_display = wl_display_create();
  /* The backend is a wlroots feature which abstracts the underlying input and
   * output hardware. The autocreate option will choose the most suitable
   * backend based on the current environment, such as opening an X11 window
   * if an X11 server is running. */
  server.backend = wlr_backend_autocreate(wl_display_get_event_loop(server.wl_display), NULL);
  if (server.backend == NULL) {
    wlr_log(WLR_ERROR, "failed to create wlr_backend");
    return 1;
  }

  /* Autocreates a renderer, either Pixman, GLES2 or Vulkan for us. The user
   * can also specify a renderer using the WLR_RENDERER env var.
   * The renderer is responsible for defining the various pixel formats it
   * supports for shared memory, this configures that for clients. */
  server.renderer = wlr_renderer_autocreate(server.backend);
  if (server.renderer == NULL) {
    wlr_log(WLR_ERROR, "failed to create wlr_renderer");
    return 1;
  }

  wlr_renderer_init_wl_display(server.renderer, server.wl_display);

  /* Autocreates an allocator for us.
   * The allocator is the bridge between the renderer and the backend. It
   * handles the buffer creation, allowing wlroots to render onto the
   * screen */
  server.allocator = wlr_allocator_autocreate(server.backend,
    server.renderer);
  if (server.allocator == NULL) {
    wlr_log(WLR_ERROR, "failed to create wlr_allocator");
    return 1;
  }

  /* This creates some hands-off wlroots interfaces. The compositor is
   * necessary for clients to allocate surfaces, the subcompositor allows to
   * assign the role of subsurfaces to surfaces and the data device manager
   * handles the clipboard. Each of these wlroots interfaces has room for you
   * to dig your fingers in and play with their behavior if you want. Note that
   * the clients cannot set the selection directly without compositor approval,
   * see the handling of the seat_request_set_selection event below.*/
  wlr_compositor_create(server.wl_display, 5, server.renderer);
  wlr_subcompositor_create(server.wl_display);
  wlr_data_device_manager_create(server.wl_display);

  /* Creates an output layout, which a wlroots utility for working with an
   * arrangement of screens in a physical layout. */
  server.output_layout = wlr_output_layout_create(server.wl_display);

  /* Configure a listener to be notified when new outputs are available on the
   * backend. */
  wl_list_init(&server.outputs);

  // Initialize the server.backend_new_output_ listener.
  server.backend_new_output_.init(&server.backend->events.new_output);
  // Request a callback to server.new_output upon the backend_new_output_ event.
  server.backend_new_output_handle_ = server.backend_new_output_.request(server, &tinywl_server::new_output);

  /* Initialize the scene graph. This is a wlroots abstraction that handles all
   * rendering and damage tracking. All the compositor author needs to do
   * is add things that should be rendered to the scene graph at the proper
   * positions and then call wlr_scene_output_commit() to render a frame if
   * necessary.
   */
  server.scene_.init();
#if 0 // FIXME: we don't have server.scene
  server.scene_layout = wlr_scene_attach_output_layout(server.scene, server.output_layout);
#endif

  /* Set up xdg-shell version 3. The xdg-shell is a Wayland protocol which is
   * used for application windows. For more detail on shells, refer to
   * https://drewdevault.com/2018/07/29/Wayland-shells.html.
   */
  wl_list_init(&server.toplevels);
  server.xdg_shell = wlr_xdg_shell_create(server.wl_display, 3);

  // Initialize the server.xdg_shell_new_toplevel_ listener.

  server.xdg_shell_new_toplevel_.init(&server.xdg_shell->events.new_toplevel);
  // Request a callback to server.xdg_shell_new_toplevel upon the xdg_shell_new_toplevel_ event.
  server.xdg_shell_new_toplevel_handle_ = server.xdg_shell_new_toplevel_.request(server, &tinywl_server::xdg_shell_new_toplevel);

  // Initialize the server.xdg_shell_new_popup_ listener.
  server.xdg_shell_new_popup_.init(&server.xdg_shell->events.new_popup);
  // Request a callback to server.xdg_shell_new_popup upon the xdg_shell_new_popup_ event.
  server.xdg_shell_new_popup_handle_ = server.xdg_shell_new_popup_.request(server, &tinywl_server::xdg_shell_new_popup);

  /*
   * Creates a cursor, which is a wlroots utility for tracking the cursor
   * image shown on screen.
   */
  server.cursor = wlr_cursor_create();
  wlr_cursor_attach_output_layout(server.cursor, server.output_layout);

  /* Creates an xcursor manager, another wlroots utility which loads up
   * Xcursor themes to source cursor images from and makes sure that cursor
   * images are available at all scale factors on the screen (necessary for
   * HiDPI support). */
  server.cursor_mgr = wlr_xcursor_manager_create(NULL, 24);

  /*
   * wlr_cursor *only* displays an image on screen. It does not move around
   * when the pointer moves. However, we can attach input devices to it, and
   * it will generate aggregate events for all of them. In these events, we
   * can choose how we want to process them, forwarding them to clients and
   * moving the cursor around. More detail on this process is described in
   * https://drewdevault.com/2018/07/17/Input-handling-in-wlroots.html.
   *
   * And more comments are sprinkled throughout the notify functions above.
   */
  server.cursor_mode = TINYWL_CURSOR_PASSTHROUGH;

  // Initialize the cursor_* event listeners
  server.cursor_motion_.init(&server.cursor->events.motion);
  server.cursor_motion_handle_ = server.cursor_motion_.request(server, &tinywl_server::cursor_motion);

  server.cursor_motion_absolute_.init(&server.cursor->events.motion_absolute);
  server.cursor_motion_absolute_handle_ = server.cursor_motion_absolute_.request(server, &tinywl_server::cursor_motion_absolute);

  server.cursor_button_.init(&server.cursor->events.button);
  server.cursor_button_handle_ = server.cursor_button_.request(server, &tinywl_server::cursor_button);

  server.cursor_axis_.init(&server.cursor->events.axis);
  server.cursor_axis_handle_ = server.cursor_axis_.request(server, &tinywl_server::cursor_axis);

  server.cursor_frame_.init(&server.cursor->events.frame);
  server.cursor_frame_handle_ = server.cursor_frame_.request(server, &tinywl_server::cursor_frame);

  /*
   * Configures a seat, which is a single "seat" at which a user sits and
   * operates the computer. This conceptually includes up to one keyboard,
   * pointer, touch, and drawing tablet device. We also rig up a listener to
   * let us know when new input devices are available on the backend.
   */
  wl_list_init(&server.keyboards);

  // Initialize the server.backend_new_input_ listener.
  server.backend_new_input_.init(&server.backend->events.new_input);
  // Request a callback to server.new_input upon the backend_new_input_ event.
  server.backend_new_input_handle_ = server.backend_new_input_.request(server, &tinywl_server::backend_new_input);

  server.seat = wlr_seat_create(server.wl_display, "seat0");

  // Initialize the seat_request_set_cursor_ listener.
  server.seat_request_set_cursor_.init(&server.seat->events.request_set_cursor);
  server.seat_request_set_cursor_handle_ = server.seat_request_set_cursor_.request(server, &tinywl_server::seat_request_set_cursor);

  // Initialize the seat_request_set_selection_ listener.
  server.seat_request_set_selection_.init(&server.seat->events.request_set_selection);
  server.seat_request_set_selection_handle_ = server.seat_request_set_selection_.request(server, &tinywl_server::seat_request_set_selection);

  /* Add a Unix socket to the Wayland display. */
  const char *socket = wl_display_add_socket_auto(server.wl_display);
  if (!socket) {
    wlr_backend_destroy(server.backend);
    return 1;
  }

  /* Start the backend. This will enumerate outputs and inputs, become the DRM
   * master, etc */
  if (!wlr_backend_start(server.backend)) {
    wlr_backend_destroy(server.backend);
    wl_display_destroy(server.wl_display);
    return 1;
  }

  /* Set the WAYLAND_DISPLAY environment variable to our socket and run the
   * startup command if requested. */
  setenv("WAYLAND_DISPLAY", socket, true);
  if (startup_cmd) {
    if (fork() == 0) {
      execl("/bin/sh", "/bin/sh", "-c", startup_cmd, (void *)NULL);
    }
  }
  /* Run the Wayland event loop. This does not return until you exit the
   * compositor. Starting the backend rigged up all of the necessary event
   * loop configuration to listen to libinput events, DRM events, generate
   * frame events at the refresh rate, and so on. */
  wlr_log(WLR_INFO, "Running Wayland compositor on WAYLAND_DISPLAY=%s",
      socket);
  wl_display_run(server.wl_display);

  /* Once wl_display_run returns, we destroy all clients then shut down the
   * server. */
  wl_display_destroy_clients(server.wl_display);

  // No need to remove signal listeners with wl::Listener - they clean up automatically

#if 0 // FIXME: we don't have server.scene
  wlr_scene_node_destroy(&server.scene->tree.node);
#endif
  wlr_xcursor_manager_destroy(server.cursor_mgr);
  wlr_cursor_destroy(server.cursor);
  wlr_allocator_destroy(server.allocator);
  wlr_renderer_destroy(server.renderer);
  wlr_backend_destroy(server.backend);
  wl_display_destroy(server.wl_display);
  return 0;
}
