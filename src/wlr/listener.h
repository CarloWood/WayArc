#pragma once

#include "wl/Listener.h"
#include "wlr/events.h"
#include <boost/preprocessor/seq/for_each.hpp>

#define DECLARE_LISTENER(r, event_class, signal_name) \
  using signal_name = wl::Listener<wlr::events::event_class::signal_name>;

#define DECLARE_LISTENERS(event_class, events_seq) \
namespace event_class { \
  BOOST_PP_SEQ_FOR_EACH(DECLARE_LISTENER, event_class, events_seq) \
}

// Declare all wl::Listener<> types in namespace listener.
//
// This file was generated. It contains a DECLARE_LISTENERS(wlr_XYZ, (signal1)(signal2)...)
// for each struct wlr_XYZ that contains an `events` struct with wl_signal members
// signal1, signal2 etc.
//
// For example,
//
// struct wlr_XYZ {
//   struct {
//     struct wl_signal signal1;
//     struct wl_signal signal2;
//   } events;
// };
namespace listener {

// These DECLARE_LISTENERS lines are generated with: events_generator/convert --listener analyzer.out :

DECLARE_LISTENERS(wlr_allocator, (destroy));
DECLARE_LISTENERS(wlr_backend, (destroy)(new_input)(new_output));
DECLARE_LISTENERS(wlr_buffer, (destroy)(release));
DECLARE_LISTENERS(wlr_compositor, (destroy)(new_surface));
DECLARE_LISTENERS(wlr_content_type_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_cursor, (axis)(button)(frame)(hold_begin)(hold_end)(motion)(motion_absolute)(pinch_begin)(pinch_end)(pinch_update)(swipe_begin)(swipe_end)(swipe_update)(tablet_tool_axis)(tablet_tool_button)(tablet_tool_proximity)(tablet_tool_tip)(touch_cancel)(touch_down)(touch_frame)(touch_motion)(touch_up));
DECLARE_LISTENERS(wlr_cursor_shape_manager_v1, (destroy)(request_set_shape));
DECLARE_LISTENERS(wlr_data_control_manager_v1, (destroy)(new_device));
DECLARE_LISTENERS(wlr_data_device_manager, (destroy));
DECLARE_LISTENERS(wlr_data_source, (destroy));
DECLARE_LISTENERS(wlr_device, (change)(remove));
DECLARE_LISTENERS(wlr_drag, (destroy)(drop)(focus)(motion));
DECLARE_LISTENERS(wlr_drag_icon, (destroy));
DECLARE_LISTENERS(wlr_drm, (destroy));
DECLARE_LISTENERS(wlr_drm_lease, (destroy));
DECLARE_LISTENERS(wlr_drm_lease_v1_manager, (destroy)(request));
DECLARE_LISTENERS(wlr_export_dmabuf_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_ext_data_control_manager_v1, (destroy)(new_device));
DECLARE_LISTENERS(wlr_ext_foreign_toplevel_handle_v1, (destroy));
DECLARE_LISTENERS(wlr_ext_foreign_toplevel_list_v1, (destroy));
DECLARE_LISTENERS(wlr_ext_image_capture_source_v1, (constraints_update)(destroy)(frame));
DECLARE_LISTENERS(wlr_ext_image_capture_source_v1_cursor, (update));
DECLARE_LISTENERS(wlr_ext_image_copy_capture_frame_v1, (destroy));
DECLARE_LISTENERS(wlr_foreign_toplevel_handle_v1, (destroy)(request_activate)(request_close)(request_fullscreen)(request_maximize)(request_minimize)(set_rectangle));
DECLARE_LISTENERS(wlr_foreign_toplevel_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_fractional_scale_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_fullscreen_shell_v1, (destroy)(present_surface));
DECLARE_LISTENERS(wlr_gamma_control_manager_v1, (destroy)(set_gamma));
DECLARE_LISTENERS(wlr_idle_inhibit_manager_v1, (destroy)(new_inhibitor));
DECLARE_LISTENERS(wlr_idle_inhibitor_v1, (destroy));
DECLARE_LISTENERS(wlr_input_device, (destroy));
DECLARE_LISTENERS(wlr_input_method_keyboard_grab_v2, (destroy));
DECLARE_LISTENERS(wlr_input_method_manager_v2, (destroy)(input_method));
DECLARE_LISTENERS(wlr_input_method_v2, (commit)(destroy)(grab_keyboard)(new_popup_surface));
DECLARE_LISTENERS(wlr_input_popup_surface_v2, (destroy));
DECLARE_LISTENERS(wlr_keyboard, (key)(keymap)(modifiers)(repeat_info));
DECLARE_LISTENERS(wlr_keyboard_group, (enter)(leave));
DECLARE_LISTENERS(wlr_keyboard_shortcuts_inhibit_manager_v1, (destroy)(new_inhibitor));
DECLARE_LISTENERS(wlr_keyboard_shortcuts_inhibitor_v1, (destroy));
DECLARE_LISTENERS(wlr_layer_shell_v1, (destroy)(new_surface));
DECLARE_LISTENERS(wlr_layer_surface_v1, (destroy)(new_popup));
DECLARE_LISTENERS(wlr_linux_dmabuf_v1, (destroy));
DECLARE_LISTENERS(wlr_multi_backend, (backend_add)(backend_remove));
DECLARE_LISTENERS(wlr_output, (bind)(commit)(damage)(description)(destroy)(frame)(needs_frame)(precommit)(present)(request_state));
DECLARE_LISTENERS(wlr_output_layer, (feedback));
DECLARE_LISTENERS(wlr_output_layout, (add)(change)(destroy));
DECLARE_LISTENERS(wlr_output_layout_output, (destroy));
DECLARE_LISTENERS(wlr_output_manager_v1, (apply)(destroy)(test));
DECLARE_LISTENERS(wlr_output_power_manager_v1, (destroy)(set_mode));
DECLARE_LISTENERS(wlr_pointer, (axis)(button)(frame)(hold_begin)(hold_end)(motion)(motion_absolute)(pinch_begin)(pinch_end)(pinch_update)(swipe_begin)(swipe_end)(swipe_update));
DECLARE_LISTENERS(wlr_pointer_constraint_v1, (destroy)(set_region));
DECLARE_LISTENERS(wlr_pointer_constraints_v1, (destroy)(new_constraint));
DECLARE_LISTENERS(wlr_pointer_gestures_v1, (destroy));
DECLARE_LISTENERS(wlr_presentation, (destroy));
DECLARE_LISTENERS(wlr_primary_selection_source, (destroy));
DECLARE_LISTENERS(wlr_primary_selection_v1_device_manager, (destroy));
DECLARE_LISTENERS(wlr_relative_pointer_manager_v1, (destroy)(new_relative_pointer));
DECLARE_LISTENERS(wlr_relative_pointer_v1, (destroy));
DECLARE_LISTENERS(wlr_renderer, (destroy)(lost));
DECLARE_LISTENERS(wlr_scene_buffer, (frame_done)(output_enter)(output_leave)(output_sample)(outputs_update));
DECLARE_LISTENERS(wlr_scene_node, (destroy));
DECLARE_LISTENERS(wlr_scene_output, (destroy));
DECLARE_LISTENERS(wlr_screencopy_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_seat, (destroy)(keyboard_grab_begin)(keyboard_grab_end)(pointer_grab_begin)(pointer_grab_end)(request_set_cursor)(request_set_primary_selection)(request_set_selection)(request_start_drag)(set_primary_selection)(set_selection)(start_drag)(touch_grab_begin)(touch_grab_end));
DECLARE_LISTENERS(wlr_seat_client, (destroy));
DECLARE_LISTENERS(wlr_seat_keyboard_state, (focus_change));
DECLARE_LISTENERS(wlr_seat_pointer_state, (focus_change));
DECLARE_LISTENERS(wlr_security_context_manager_v1, (commit)(destroy));
DECLARE_LISTENERS(wlr_server_decoration, (destroy)(mode));
DECLARE_LISTENERS(wlr_server_decoration_manager, (destroy)(new_decoration));
DECLARE_LISTENERS(wlr_session, (active)(add_drm_card)(destroy));
DECLARE_LISTENERS(wlr_session_lock_manager_v1, (destroy)(new_lock));
DECLARE_LISTENERS(wlr_session_lock_surface_v1, (destroy));
DECLARE_LISTENERS(wlr_session_lock_v1, (destroy)(new_surface)(unlock));
DECLARE_LISTENERS(wlr_subcompositor, (destroy));
DECLARE_LISTENERS(wlr_subsurface, (destroy));
DECLARE_LISTENERS(wlr_surface, (client_commit)(commit)(destroy)(map)(new_subsurface)(unmap));
DECLARE_LISTENERS(wlr_switch, (toggle));
DECLARE_LISTENERS(wlr_tablet, (axis)(button)(proximity)(tip));
DECLARE_LISTENERS(wlr_tablet_manager_v2, (destroy));
DECLARE_LISTENERS(wlr_tablet_pad, (attach_tablet)(button)(ring)(strip));
DECLARE_LISTENERS(wlr_tablet_tool, (destroy));
DECLARE_LISTENERS(wlr_tablet_v2_tablet_pad, (button_feedback)(ring_feedback)(strip_feedback));
DECLARE_LISTENERS(wlr_tablet_v2_tablet_tool, (set_cursor));
DECLARE_LISTENERS(wlr_tearing_control_manager_v1, (destroy)(new_object));
DECLARE_LISTENERS(wlr_tearing_control_v1, (destroy)(set_hint));
DECLARE_LISTENERS(wlr_text_input_manager_v3, (destroy)(text_input));
DECLARE_LISTENERS(wlr_text_input_v3, (commit)(destroy)(disable)(enable));
DECLARE_LISTENERS(wlr_touch, (cancel)(down)(frame)(motion)(up));
DECLARE_LISTENERS(wlr_touch_point, (destroy));
DECLARE_LISTENERS(wlr_transient_seat_manager_v1, (create_seat)(destroy));
DECLARE_LISTENERS(wlr_viewporter, (destroy));
DECLARE_LISTENERS(wlr_virtual_keyboard_manager_v1, (destroy)(new_virtual_keyboard));
DECLARE_LISTENERS(wlr_virtual_pointer_manager_v1, (destroy)(new_virtual_pointer));
DECLARE_LISTENERS(wlr_xdg_activation_token_v1, (destroy));
DECLARE_LISTENERS(wlr_xdg_activation_v1, (destroy)(new_token)(request_activate));
DECLARE_LISTENERS(wlr_xdg_decoration_manager_v1, (destroy)(new_toplevel_decoration));
DECLARE_LISTENERS(wlr_xdg_dialog_v1, (destroy)(set_modal));
DECLARE_LISTENERS(wlr_xdg_foreign_exported, (destroy));
DECLARE_LISTENERS(wlr_xdg_foreign_registry, (destroy));
DECLARE_LISTENERS(wlr_xdg_foreign_v1, (destroy));
DECLARE_LISTENERS(wlr_xdg_foreign_v2, (destroy));
DECLARE_LISTENERS(wlr_xdg_output_manager_v1, (destroy));
DECLARE_LISTENERS(wlr_xdg_popup, (destroy)(reposition));
DECLARE_LISTENERS(wlr_xdg_shell, (destroy)(new_popup)(new_surface)(new_toplevel));
DECLARE_LISTENERS(wlr_xdg_surface, (ack_configure)(configure)(destroy)(new_popup)(ping_timeout));
DECLARE_LISTENERS(wlr_xdg_toplevel, (destroy)(request_fullscreen)(request_maximize)(request_minimize)(request_move)(request_resize)(request_show_window_menu)(set_app_id)(set_parent)(set_title));
DECLARE_LISTENERS(wlr_xdg_toplevel_decoration_v1, (destroy)(request_mode));
DECLARE_LISTENERS(wlr_xdg_toplevel_icon_manager_v1, (destroy)(set_icon));
DECLARE_LISTENERS(wlr_xdg_wm_dialog_v1, (destroy)(new_dialog));
DECLARE_LISTENERS(wlr_xwayland, (destroy)(new_surface)(ready)(remove_startup_info));
DECLARE_LISTENERS(wlr_xwayland_server, (destroy)(ready)(start));
DECLARE_LISTENERS(wlr_xwayland_shell_v1, (destroy)(new_surface));
DECLARE_LISTENERS(wlr_xwayland_surface, (associate)(destroy)(dissociate)(focus_in)(grab_focus)(map_request)(ping_timeout)(request_above)(request_activate)(request_below)(request_close)(request_configure)(request_demands_attention)(request_fullscreen)(request_maximize)(request_minimize)(request_move)(request_resize)(request_shaded)(request_skip_pager)(request_skip_taskbar)(request_sticky)(set_class)(set_decorations)(set_geometry)(set_hints)(set_opacity)(set_override_redirect)(set_parent)(set_role)(set_startup_id)(set_strut_partial)(set_title)(set_window_type));

} // namespace listener
