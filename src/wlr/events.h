#pragma once

#include <cstddef>      // std::nullptr_t

// If the struct wlr_XYZ contains
//
//   struct wlr_XYZ {
//     struct {
//       wlr_signal destroy;
//       wlr_signal new_input;
//     } events;
//   };
//
// then this file contains
//
//   namespace wlr_XYZ {
//     using destroy = <destroy_data_type>;
//     using new_input = <new_input_data_type>;
//   } // namespace wlr_XYZ
//
// where destroy_data_type / new_input_data_type is the type of the
// POD object that `data` points to in the callback of that
// event. If that type is `std::nullptr_t` than `data` will be NULL.
//

// Everything below this line is generated with: generate_events/convert --events analyzer.out :

// Forward declare all wlroots structs.
extern "C" {
struct wlr_backend;
struct wlr_input_device;
struct wlr_output;
struct wlr_surface;
struct wlr_pointer_axis_event;
struct wlr_pointer_button_event;
struct wlr_cursor;
struct wlr_pointer_hold_begin_event;
struct wlr_pointer_hold_end_event;
struct wlr_pointer_motion_event;
struct wlr_pointer_motion_absolute_event;
struct wlr_pointer_pinch_begin_event;
struct wlr_pointer_pinch_end_event;
struct wlr_pointer_pinch_update_event;
struct wlr_pointer_swipe_begin_event;
struct wlr_pointer_swipe_end_event;
struct wlr_pointer_swipe_update_event;
struct wlr_tablet_tool_axis_event;
struct wlr_tablet_tool_button_event;
struct wlr_tablet_tool_proximity_event;
struct wlr_tablet_tool_tip_event;
struct wlr_touch_cancel_event;
struct wlr_touch_down_event;
struct wlr_touch_motion_event;
struct wlr_touch_up_event;
struct wlr_cursor_shape_manager_v1_request_set_shape_event;
struct wlr_data_control_manager_v1;
struct wlr_data_control_device_v1;
struct wlr_data_device_manager;
struct wlr_data_source;
struct wlr_device_change_event;
struct wlr_drag;
struct wlr_drag_drop_event;
struct wlr_drag_motion_event;
struct wlr_drag_icon;
struct wlr_drm_lease_request_v1;
struct wlr_export_dmabuf_manager_v1;
struct wlr_ext_data_control_manager_v1;
struct wlr_ext_data_control_device_v1;
struct wlr_ext_output_image_capture_source_v1_frame_event;
struct wlr_foreign_toplevel_handle_v1;
struct wlr_foreign_toplevel_handle_v1_activated_event;
struct wlr_foreign_toplevel_handle_v1_fullscreen_event;
struct wlr_foreign_toplevel_handle_v1_maximized_event;
struct wlr_foreign_toplevel_handle_v1_minimized_event;
struct wlr_foreign_toplevel_handle_v1_set_rectangle_event;
struct wlr_foreign_toplevel_manager_v1;
struct wlr_fullscreen_shell_v1;
struct wlr_fullscreen_shell_v1_present_surface_event;
struct wlr_gamma_control_manager_v1;
struct wlr_gamma_control_manager_v1_set_gamma_event;
struct wlr_idle_inhibit_manager_v1;
struct wlr_idle_inhibitor_v1;
struct wlr_input_method_keyboard_grab_v2;
struct wlr_input_method_manager_v2;
struct wlr_input_method_v2;
struct wlr_input_popup_surface_v2;
struct wlr_keyboard_key_event;
struct wlr_keyboard;
struct wl_array;
struct wlr_keyboard_shortcuts_inhibit_manager_v1;
struct wlr_keyboard_shortcuts_inhibitor_v1;
struct wlr_layer_shell_v1;
struct wlr_layer_surface_v1;
struct wlr_xdg_popup;
struct wlr_linux_dmabuf_v1;
struct wlr_output_event_bind;
struct wlr_output_event_commit;
struct wlr_output_event_damage;
struct wlr_output_event_precommit;
struct wlr_output_event_present;
struct wlr_output_event_request_state;
struct wlr_output_layer_feedback_event;
struct wlr_output_layout_output;
struct wlr_output_layout;
struct wlr_output_configuration_v1;
struct wlr_output_manager_v1;
struct wlr_output_power_manager_v1;
struct wlr_output_power_v1_set_mode_event;
struct wlr_pointer;
struct wlr_pointer_constraint_v1;
struct wlr_presentation;
struct wlr_primary_selection_source;
struct wlr_primary_selection_v1_device_manager;
struct wlr_relative_pointer_manager_v1;
struct wlr_relative_pointer_v1;
struct wlr_renderer;
struct timespec;
struct wlr_scene_output;
struct wlr_scene_output_sample_event;
struct wlr_scene_outputs_update_event;
struct wlr_screencopy_manager_v1;
struct wlr_seat;
struct wlr_seat_keyboard_grab;
struct wlr_seat_pointer_grab;
struct wlr_seat_pointer_request_set_cursor_event;
struct wlr_seat_request_set_primary_selection_event;
struct wlr_seat_request_set_selection_event;
struct wlr_seat_request_start_drag_event;
struct wlr_seat_touch_grab;
struct wlr_seat_client;
struct wlr_seat_keyboard_focus_change_event;
struct wlr_seat_pointer_focus_change_event;
struct wlr_security_context_v1_commit_event;
struct wlr_security_context_manager_v1;
struct wlr_server_decoration;
struct wlr_server_decoration_manager;
struct wlr_session_add_event;
struct wlr_session;
struct wlr_session_lock_v1;
struct wlr_session_lock_surface_v1;
struct wlr_subsurface;
struct wlr_switch_toggle_event;
struct wlr_tablet_manager_v2;
struct wlr_tablet_tool;
struct wlr_tablet_pad_button_event;
struct wlr_tablet_pad_ring_event;
struct wlr_tablet_pad_strip_event;
struct wlr_tablet_v2_event_feedback;
struct wlr_tablet_v2_event_cursor;
struct wlr_tearing_control_v1;
struct wlr_text_input_manager_v3;
struct wlr_text_input_v3;
struct wlr_touch_point;
struct wlr_transient_seat_v1;
struct wlr_virtual_keyboard_manager_v1;
struct wlr_virtual_keyboard_v1;
struct wlr_virtual_pointer_manager_v1;
struct wlr_virtual_pointer_v1_new_pointer_event;
struct wlr_xdg_activation_token_v1;
struct wlr_xdg_activation_v1_request_activate_event;
struct wlr_xdg_decoration_manager_v1;
struct wlr_xdg_toplevel_decoration_v1;
struct wlr_xdg_output_manager_v1;
struct wlr_xdg_shell;
struct wlr_xdg_surface;
struct wlr_xdg_toplevel;
struct wlr_xdg_surface_configure;
struct wlr_xdg_toplevel_move_event;
struct wlr_xdg_toplevel_resize_event;
struct wlr_xdg_toplevel_show_window_menu_event;
struct wlr_xdg_toplevel_icon_manager_v1_set_icon_event;
struct wlr_xdg_dialog_v1;
struct wlr_xwayland_surface;
struct wlr_xwayland_remove_startup_info_event;
struct wlr_xwayland_server_ready_event;
struct wlr_xwayland_surface_v1;
struct wlr_xwayland_surface_configure_event;
struct wlr_xwayland_minimize_event;
struct wlr_xwayland_resize_event;
} // extern "C"

namespace wlr::events {
namespace wlr_allocator {
  using destroy = std::nullptr_t;
} // namespace wlr_allocator
namespace wlr_backend {
  using destroy = struct wlr_backend;
  using new_input = struct wlr_input_device;
  using new_output = struct wlr_output;
} // namespace wlr_backend
namespace wlr_buffer {
  using destroy = std::nullptr_t;
  using release = std::nullptr_t;
} // namespace wlr_buffer
namespace wlr_compositor {
  using destroy = std::nullptr_t;
  using new_surface = struct wlr_surface;
} // namespace wlr_compositor
namespace wlr_content_type_manager_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_content_type_manager_v1
namespace wlr_cursor {
  using axis = struct wlr_pointer_axis_event;
  using button = struct wlr_pointer_button_event;
  using frame = struct wlr_cursor;
  using hold_begin = struct wlr_pointer_hold_begin_event;
  using hold_end = struct wlr_pointer_hold_end_event;
  using motion = struct wlr_pointer_motion_event;
  using motion_absolute = struct wlr_pointer_motion_absolute_event;
  using pinch_begin = struct wlr_pointer_pinch_begin_event;
  using pinch_end = struct wlr_pointer_pinch_end_event;
  using pinch_update = struct wlr_pointer_pinch_update_event;
  using swipe_begin = struct wlr_pointer_swipe_begin_event;
  using swipe_end = struct wlr_pointer_swipe_end_event;
  using swipe_update = struct wlr_pointer_swipe_update_event;
  using tablet_tool_axis = struct wlr_tablet_tool_axis_event;
  using tablet_tool_button = struct wlr_tablet_tool_button_event;
  using tablet_tool_proximity = struct wlr_tablet_tool_proximity_event;
  using tablet_tool_tip = struct wlr_tablet_tool_tip_event;
  using touch_cancel = struct wlr_touch_cancel_event;
  using touch_down = struct wlr_touch_down_event;
  using touch_frame = std::nullptr_t;
  using touch_motion = struct wlr_touch_motion_event;
  using touch_up = struct wlr_touch_up_event;
} // namespace wlr_cursor
namespace wlr_cursor_shape_manager_v1 {
  using destroy = std::nullptr_t;
  using request_set_shape = struct wlr_cursor_shape_manager_v1_request_set_shape_event;
} // namespace wlr_cursor_shape_manager_v1
namespace wlr_data_control_manager_v1 {
  using destroy = struct wlr_data_control_manager_v1;
  using new_device = struct wlr_data_control_device_v1;
} // namespace wlr_data_control_manager_v1
namespace wlr_data_device_manager {
  using destroy = struct wlr_data_device_manager;
} // namespace wlr_data_device_manager
namespace wlr_data_source {
  using destroy = struct wlr_data_source;
} // namespace wlr_data_source
namespace wlr_device {
  using change = struct wlr_device_change_event;
  using remove = std::nullptr_t;
} // namespace wlr_device
namespace wlr_drag {
  using destroy = struct wlr_drag;
  using drop = struct wlr_drag_drop_event;
  using focus = struct wlr_drag;
  using motion = struct wlr_drag_motion_event;
} // namespace wlr_drag
namespace wlr_drag_icon {
  using destroy = struct wlr_drag_icon;
} // namespace wlr_drag_icon
namespace wlr_drm {
  using destroy = std::nullptr_t;
} // namespace wlr_drm
namespace wlr_drm_lease {
  using destroy = std::nullptr_t;
} // namespace wlr_drm_lease
namespace wlr_drm_lease_v1_manager {
  using destroy = std::nullptr_t;
  using request = struct wlr_drm_lease_request_v1;
} // namespace wlr_drm_lease_v1_manager
namespace wlr_export_dmabuf_manager_v1 {
  using destroy = struct wlr_export_dmabuf_manager_v1;
} // namespace wlr_export_dmabuf_manager_v1
namespace wlr_ext_data_control_manager_v1 {
  using destroy = struct wlr_ext_data_control_manager_v1;
  using new_device = struct wlr_ext_data_control_device_v1;
} // namespace wlr_ext_data_control_manager_v1
namespace wlr_ext_foreign_toplevel_handle_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_ext_foreign_toplevel_handle_v1
namespace wlr_ext_foreign_toplevel_list_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_ext_foreign_toplevel_list_v1
namespace wlr_ext_image_capture_source_v1 {
  using constraints_update = std::nullptr_t;
  using destroy = std::nullptr_t;
  using frame = struct wlr_ext_output_image_capture_source_v1_frame_event;
} // namespace wlr_ext_image_capture_source_v1
namespace wlr_ext_image_capture_source_v1_cursor {
  using update = std::nullptr_t;
} // namespace wlr_ext_image_capture_source_v1_cursor
namespace wlr_ext_image_copy_capture_frame_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_ext_image_copy_capture_frame_v1
namespace wlr_foreign_toplevel_handle_v1 {
  using destroy = struct wlr_foreign_toplevel_handle_v1;
  using request_activate = struct wlr_foreign_toplevel_handle_v1_activated_event;
  using request_close = struct wlr_foreign_toplevel_handle_v1;
  using request_fullscreen = struct wlr_foreign_toplevel_handle_v1_fullscreen_event;
  using request_maximize = struct wlr_foreign_toplevel_handle_v1_maximized_event;
  using request_minimize = struct wlr_foreign_toplevel_handle_v1_minimized_event;
  using set_rectangle = struct wlr_foreign_toplevel_handle_v1_set_rectangle_event;
} // namespace wlr_foreign_toplevel_handle_v1
namespace wlr_foreign_toplevel_manager_v1 {
  using destroy = struct wlr_foreign_toplevel_manager_v1;
} // namespace wlr_foreign_toplevel_manager_v1
namespace wlr_fractional_scale_manager_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_fractional_scale_manager_v1
namespace wlr_fullscreen_shell_v1 {
  using destroy = struct wlr_fullscreen_shell_v1;
  using present_surface = struct wlr_fullscreen_shell_v1_present_surface_event;
} // namespace wlr_fullscreen_shell_v1
namespace wlr_gamma_control_manager_v1 {
  using destroy = struct wlr_gamma_control_manager_v1;
  using set_gamma = struct wlr_gamma_control_manager_v1_set_gamma_event;
} // namespace wlr_gamma_control_manager_v1
namespace wlr_idle_inhibit_manager_v1 {
  using destroy = struct wlr_idle_inhibit_manager_v1;
  using new_inhibitor = struct wlr_idle_inhibitor_v1;
} // namespace wlr_idle_inhibit_manager_v1
namespace wlr_idle_inhibitor_v1 {
  using destroy = struct wlr_surface;
} // namespace wlr_idle_inhibitor_v1
namespace wlr_input_device {
  using destroy = struct wlr_input_device;
} // namespace wlr_input_device
namespace wlr_input_method_keyboard_grab_v2 {
  using destroy = struct wlr_input_method_keyboard_grab_v2;
} // namespace wlr_input_method_keyboard_grab_v2
namespace wlr_input_method_manager_v2 {
  using destroy = struct wlr_input_method_manager_v2;
  using input_method = struct wlr_input_method_v2;
} // namespace wlr_input_method_manager_v2
namespace wlr_input_method_v2 {
  using commit = struct wlr_input_method_v2;
  using destroy = struct wlr_input_method_v2;
  using grab_keyboard = struct wlr_input_method_keyboard_grab_v2;
  using new_popup_surface = struct wlr_input_popup_surface_v2;
} // namespace wlr_input_method_v2
namespace wlr_input_popup_surface_v2 {
  using destroy = std::nullptr_t;
} // namespace wlr_input_popup_surface_v2
namespace wlr_keyboard {
  using key = struct wlr_keyboard_key_event;
  using keymap = struct wlr_keyboard;
  using modifiers = struct wlr_keyboard;
  using repeat_info = struct wlr_keyboard;
} // namespace wlr_keyboard
namespace wlr_keyboard_group {
  using enter = struct wl_array;
  using leave = struct wl_array;
} // namespace wlr_keyboard_group
namespace wlr_keyboard_shortcuts_inhibit_manager_v1 {
  using destroy = struct wlr_keyboard_shortcuts_inhibit_manager_v1;
  using new_inhibitor = struct wlr_keyboard_shortcuts_inhibitor_v1;
} // namespace wlr_keyboard_shortcuts_inhibit_manager_v1
namespace wlr_keyboard_shortcuts_inhibitor_v1 {
  using destroy = struct wlr_keyboard_shortcuts_inhibitor_v1;
} // namespace wlr_keyboard_shortcuts_inhibitor_v1
namespace wlr_layer_shell_v1 {
  using destroy = struct wlr_layer_shell_v1;
  using new_surface = struct wlr_layer_surface_v1;
} // namespace wlr_layer_shell_v1
namespace wlr_layer_surface_v1 {
  using destroy = struct wlr_layer_surface_v1;
  using new_popup = struct wlr_xdg_popup;
} // namespace wlr_layer_surface_v1
namespace wlr_linux_dmabuf_v1 {
  using destroy = struct wlr_linux_dmabuf_v1;
} // namespace wlr_linux_dmabuf_v1
namespace wlr_multi_backend {
  using backend_add = struct wlr_backend;
  using backend_remove = struct wlr_backend;
} // namespace wlr_multi_backend
namespace wlr_output {
  using bind = struct wlr_output_event_bind;
  using commit = struct wlr_output_event_commit;
  using damage = struct wlr_output_event_damage;
  using description = struct wlr_output;
  using destroy = struct wlr_output;
  using frame = struct wlr_output;
  using needs_frame = struct wlr_output;
  using precommit = struct wlr_output_event_precommit;
  using present = struct wlr_output_event_present;
  using request_state = struct wlr_output_event_request_state;
} // namespace wlr_output
namespace wlr_output_layer {
  using feedback = struct wlr_output_layer_feedback_event;
} // namespace wlr_output_layer
namespace wlr_output_layout {
  using add = struct wlr_output_layout_output;
  using change = struct wlr_output_layout;
  using destroy = struct wlr_output_layout;
} // namespace wlr_output_layout
namespace wlr_output_layout_output {
  using destroy = struct wlr_output_layout_output;
} // namespace wlr_output_layout_output
namespace wlr_output_manager_v1 {
  using apply = struct wlr_output_configuration_v1;
  using destroy = struct wlr_output_manager_v1;
  using test = struct wlr_output_configuration_v1;
} // namespace wlr_output_manager_v1
namespace wlr_output_power_manager_v1 {
  using destroy = struct wlr_output_power_manager_v1;
  using set_mode = struct wlr_output_power_v1_set_mode_event;
} // namespace wlr_output_power_manager_v1
namespace wlr_pointer {
  using axis = struct wlr_pointer_axis_event;
  using button = struct wlr_pointer_button_event;
  using frame = struct wlr_pointer;
  using hold_begin = struct wlr_pointer_hold_begin_event;
  using hold_end = struct wlr_pointer_hold_end_event;
  using motion = struct wlr_pointer_motion_event;
  using motion_absolute = struct wlr_pointer_motion_absolute_event;
  using pinch_begin = struct wlr_pointer_pinch_begin_event;
  using pinch_end = struct wlr_pointer_pinch_end_event;
  using pinch_update = struct wlr_pointer_pinch_update_event;
  using swipe_begin = struct wlr_pointer_swipe_begin_event;
  using swipe_end = struct wlr_pointer_swipe_end_event;
  using swipe_update = struct wlr_pointer_swipe_update_event;
} // namespace wlr_pointer
namespace wlr_pointer_constraint_v1 {
  using destroy = struct wlr_pointer_constraint_v1;
  using set_region = std::nullptr_t;
} // namespace wlr_pointer_constraint_v1
namespace wlr_pointer_constraints_v1 {
  using destroy = std::nullptr_t;
  using new_constraint = struct wlr_pointer_constraint_v1;
} // namespace wlr_pointer_constraints_v1
namespace wlr_pointer_gestures_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_pointer_gestures_v1
namespace wlr_presentation {
  using destroy = struct wlr_presentation;
} // namespace wlr_presentation
namespace wlr_primary_selection_source {
  using destroy = struct wlr_primary_selection_source;
} // namespace wlr_primary_selection_source
namespace wlr_primary_selection_v1_device_manager {
  using destroy = struct wlr_primary_selection_v1_device_manager;
} // namespace wlr_primary_selection_v1_device_manager
namespace wlr_relative_pointer_manager_v1 {
  using destroy = struct wlr_relative_pointer_manager_v1;
  using new_relative_pointer = struct wlr_relative_pointer_v1;
} // namespace wlr_relative_pointer_manager_v1
namespace wlr_relative_pointer_v1 {
  using destroy = struct wlr_relative_pointer_v1;
} // namespace wlr_relative_pointer_v1
namespace wlr_renderer {
  using destroy = struct wlr_renderer;
  using lost = std::nullptr_t;
} // namespace wlr_renderer
namespace wlr_scene_buffer {
  using frame_done = struct timespec;
  using output_enter = struct wlr_scene_output;
  using output_leave = struct wlr_scene_output;
  using output_sample = struct wlr_scene_output_sample_event;
  using outputs_update = struct wlr_scene_outputs_update_event;
} // namespace wlr_scene_buffer
namespace wlr_scene_node {
  using destroy = std::nullptr_t;
} // namespace wlr_scene_node
namespace wlr_scene_output {
  using destroy = std::nullptr_t;
} // namespace wlr_scene_output
namespace wlr_screencopy_manager_v1 {
  using destroy = struct wlr_screencopy_manager_v1;
} // namespace wlr_screencopy_manager_v1
namespace wlr_seat {
  using destroy = struct wlr_seat;
  using keyboard_grab_begin = struct wlr_seat_keyboard_grab;
  using keyboard_grab_end = struct wlr_seat_keyboard_grab;
  using pointer_grab_begin = struct wlr_seat_pointer_grab;
  using pointer_grab_end = struct wlr_seat_pointer_grab;
  using request_set_cursor = struct wlr_seat_pointer_request_set_cursor_event;
  using request_set_primary_selection = struct wlr_seat_request_set_primary_selection_event;
  using request_set_selection = struct wlr_seat_request_set_selection_event;
  using request_start_drag = struct wlr_seat_request_start_drag_event;
  using set_primary_selection = struct wlr_seat;
  using set_selection = struct wlr_seat;
  using start_drag = struct wlr_drag;
  using touch_grab_begin = struct wlr_seat_touch_grab;
  using touch_grab_end = struct wlr_seat_touch_grab;
} // namespace wlr_seat
namespace wlr_seat_client {
  using destroy = struct wlr_seat_client;
} // namespace wlr_seat_client
namespace wlr_seat_keyboard_state {
  using focus_change = struct wlr_seat_keyboard_focus_change_event;
} // namespace wlr_seat_keyboard_state
namespace wlr_seat_pointer_state {
  using focus_change = struct wlr_seat_pointer_focus_change_event;
} // namespace wlr_seat_pointer_state
namespace wlr_security_context_manager_v1 {
  using commit = struct wlr_security_context_v1_commit_event;
  using destroy = struct wlr_security_context_manager_v1;
} // namespace wlr_security_context_manager_v1
namespace wlr_server_decoration {
  using destroy = struct wlr_server_decoration;
  using mode = struct wlr_server_decoration;
} // namespace wlr_server_decoration
namespace wlr_server_decoration_manager {
  using destroy = struct wlr_server_decoration_manager;
  using new_decoration = struct wlr_server_decoration;
} // namespace wlr_server_decoration_manager
namespace wlr_session {
  using active = std::nullptr_t;
  using add_drm_card = struct wlr_session_add_event;
  using destroy = struct wlr_session;
} // namespace wlr_session
namespace wlr_session_lock_manager_v1 {
  using destroy = std::nullptr_t;
  using new_lock = struct wlr_session_lock_v1;
} // namespace wlr_session_lock_manager_v1
namespace wlr_session_lock_surface_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_session_lock_surface_v1
namespace wlr_session_lock_v1 {
  using destroy = std::nullptr_t;
  using new_surface = struct wlr_session_lock_surface_v1;
  using unlock = std::nullptr_t;
} // namespace wlr_session_lock_v1
namespace wlr_subcompositor {
  using destroy = std::nullptr_t;
} // namespace wlr_subcompositor
namespace wlr_subsurface {
  using destroy = struct wlr_subsurface;
} // namespace wlr_subsurface
namespace wlr_surface {
  using client_commit = std::nullptr_t;
  using commit = struct wlr_surface;
  using destroy = struct wlr_surface;
  using map = std::nullptr_t;
  using new_subsurface = struct wlr_subsurface;
  using unmap = std::nullptr_t;
} // namespace wlr_surface
namespace wlr_switch {
  using toggle = struct wlr_switch_toggle_event;
} // namespace wlr_switch
namespace wlr_tablet {
  using axis = struct wlr_tablet_tool_axis_event;
  using button = struct wlr_tablet_tool_button_event;
  using proximity = struct wlr_tablet_tool_proximity_event;
  using tip = struct wlr_tablet_tool_tip_event;
} // namespace wlr_tablet
namespace wlr_tablet_manager_v2 {
  using destroy = struct wlr_tablet_manager_v2;
} // namespace wlr_tablet_manager_v2
namespace wlr_tablet_pad {
  using attach_tablet = struct wlr_tablet_tool;
  using button = struct wlr_tablet_pad_button_event;
  using ring = struct wlr_tablet_pad_ring_event;
  using strip = struct wlr_tablet_pad_strip_event;
} // namespace wlr_tablet_pad
namespace wlr_tablet_tool {
  using destroy = struct wlr_tablet_tool;
} // namespace wlr_tablet_tool
namespace wlr_tablet_v2_tablet_pad {
  using button_feedback = struct wlr_tablet_v2_event_feedback;
  using ring_feedback = struct wlr_tablet_v2_event_feedback;
  using strip_feedback = struct wlr_tablet_v2_event_feedback;
} // namespace wlr_tablet_v2_tablet_pad
namespace wlr_tablet_v2_tablet_tool {
  using set_cursor = struct wlr_tablet_v2_event_cursor;
} // namespace wlr_tablet_v2_tablet_tool
namespace wlr_tearing_control_manager_v1 {
  using destroy = std::nullptr_t;
  using new_object = struct wlr_tearing_control_v1;
} // namespace wlr_tearing_control_manager_v1
namespace wlr_tearing_control_v1 {
  using destroy = std::nullptr_t;
  using set_hint = std::nullptr_t;
} // namespace wlr_tearing_control_v1
namespace wlr_text_input_manager_v3 {
  using destroy = struct wlr_text_input_manager_v3;
  using text_input = struct wlr_text_input_v3;
} // namespace wlr_text_input_manager_v3
namespace wlr_text_input_v3 {
  using commit = struct wlr_text_input_v3;
  using destroy = struct wlr_text_input_v3;
  using disable = struct wlr_text_input_v3;
  using enable = struct wlr_text_input_v3;
} // namespace wlr_text_input_v3
namespace wlr_touch {
  using cancel = struct wlr_touch_cancel_event;
  using down = struct wlr_touch_down_event;
  using frame = std::nullptr_t;
  using motion = struct wlr_touch_motion_event;
  using up = struct wlr_touch_up_event;
} // namespace wlr_touch
namespace wlr_touch_point {
  using destroy = struct wlr_touch_point;
} // namespace wlr_touch_point
namespace wlr_transient_seat_manager_v1 {
  using create_seat = struct wlr_transient_seat_v1;
  using destroy = std::nullptr_t;
} // namespace wlr_transient_seat_manager_v1
namespace wlr_viewporter {
  using destroy = std::nullptr_t;
} // namespace wlr_viewporter
namespace wlr_virtual_keyboard_manager_v1 {
  using destroy = struct wlr_virtual_keyboard_manager_v1;
  using new_virtual_keyboard = struct wlr_virtual_keyboard_v1;
} // namespace wlr_virtual_keyboard_manager_v1
namespace wlr_virtual_pointer_manager_v1 {
  using destroy = struct wlr_virtual_pointer_manager_v1;
  using new_virtual_pointer = struct wlr_virtual_pointer_v1_new_pointer_event;
} // namespace wlr_virtual_pointer_manager_v1
namespace wlr_xdg_activation_token_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_xdg_activation_token_v1
namespace wlr_xdg_activation_v1 {
  using destroy = std::nullptr_t;
  using new_token = struct wlr_xdg_activation_token_v1;
  using request_activate = struct wlr_xdg_activation_v1_request_activate_event;
} // namespace wlr_xdg_activation_v1
namespace wlr_xdg_decoration_manager_v1 {
  using destroy = struct wlr_xdg_decoration_manager_v1;
  using new_toplevel_decoration = struct wlr_xdg_toplevel_decoration_v1;
} // namespace wlr_xdg_decoration_manager_v1
namespace wlr_xdg_dialog_v1 {
  using destroy = std::nullptr_t;
  using set_modal = std::nullptr_t;
} // namespace wlr_xdg_dialog_v1
namespace wlr_xdg_foreign_exported {
  using destroy = std::nullptr_t;
} // namespace wlr_xdg_foreign_exported
namespace wlr_xdg_foreign_registry {
  using destroy = std::nullptr_t;
} // namespace wlr_xdg_foreign_registry
namespace wlr_xdg_foreign_v1 {
  using destroy = std::nullptr_t;
} // namespace wlr_xdg_foreign_v1
namespace wlr_xdg_foreign_v2 {
  using destroy = std::nullptr_t;
} // namespace wlr_xdg_foreign_v2
namespace wlr_xdg_output_manager_v1 {
  using destroy = struct wlr_xdg_output_manager_v1;
} // namespace wlr_xdg_output_manager_v1
namespace wlr_xdg_popup {
  using destroy = std::nullptr_t;
  using reposition = std::nullptr_t;
} // namespace wlr_xdg_popup
namespace wlr_xdg_shell {
  using destroy = struct wlr_xdg_shell;
  using new_popup = struct wlr_xdg_popup;
  using new_surface = struct wlr_xdg_surface;
  using new_toplevel = struct wlr_xdg_toplevel;
} // namespace wlr_xdg_shell
namespace wlr_xdg_surface {
  using ack_configure = struct wlr_xdg_surface_configure;
  using configure = struct wlr_xdg_surface_configure;
  using destroy = std::nullptr_t;
  using new_popup = struct wlr_xdg_popup;
  using ping_timeout = std::nullptr_t;
} // namespace wlr_xdg_surface
namespace wlr_xdg_toplevel {
  using destroy = std::nullptr_t;
  using request_fullscreen = std::nullptr_t;
  using request_maximize = std::nullptr_t;
  using request_minimize = std::nullptr_t;
  using request_move = struct wlr_xdg_toplevel_move_event;
  using request_resize = struct wlr_xdg_toplevel_resize_event;
  using request_show_window_menu = struct wlr_xdg_toplevel_show_window_menu_event;
  using set_app_id = std::nullptr_t;
  using set_parent = std::nullptr_t;
  using set_title = std::nullptr_t;
} // namespace wlr_xdg_toplevel
namespace wlr_xdg_toplevel_decoration_v1 {
  using destroy = struct wlr_xdg_toplevel_decoration_v1;
  using request_mode = struct wlr_xdg_toplevel_decoration_v1;
} // namespace wlr_xdg_toplevel_decoration_v1
namespace wlr_xdg_toplevel_icon_manager_v1 {
  using destroy = std::nullptr_t;
  using set_icon = struct wlr_xdg_toplevel_icon_manager_v1_set_icon_event;
} // namespace wlr_xdg_toplevel_icon_manager_v1
namespace wlr_xdg_wm_dialog_v1 {
  using destroy = std::nullptr_t;
  using new_dialog = struct wlr_xdg_dialog_v1;
} // namespace wlr_xdg_wm_dialog_v1
namespace wlr_xwayland {
  using destroy = std::nullptr_t;
  using new_surface = struct wlr_xwayland_surface;
  using ready = std::nullptr_t;
  using remove_startup_info = struct wlr_xwayland_remove_startup_info_event;
} // namespace wlr_xwayland
namespace wlr_xwayland_server {
  using destroy = std::nullptr_t;
  using ready = struct wlr_xwayland_server_ready_event;
  using start = std::nullptr_t;
} // namespace wlr_xwayland_server
namespace wlr_xwayland_shell_v1 {
  using destroy = std::nullptr_t;
  using new_surface = struct wlr_xwayland_surface_v1;
} // namespace wlr_xwayland_shell_v1
namespace wlr_xwayland_surface {
  using associate = std::nullptr_t;
  using destroy = std::nullptr_t;
  using dissociate = std::nullptr_t;
  using focus_in = std::nullptr_t;
  using grab_focus = std::nullptr_t;
  using map_request = std::nullptr_t;
  using ping_timeout = std::nullptr_t;
  using request_above = std::nullptr_t;
  using request_activate = std::nullptr_t;
  using request_below = std::nullptr_t;
  using request_close = std::nullptr_t;
  using request_configure = struct wlr_xwayland_surface_configure_event;
  using request_demands_attention = std::nullptr_t;
  using request_fullscreen = std::nullptr_t;
  using request_maximize = std::nullptr_t;
  using request_minimize = struct wlr_xwayland_minimize_event;
  using request_move = std::nullptr_t;
  using request_resize = struct wlr_xwayland_resize_event;
  using request_shaded = std::nullptr_t;
  using request_skip_pager = std::nullptr_t;
  using request_skip_taskbar = std::nullptr_t;
  using request_sticky = std::nullptr_t;
  using set_class = std::nullptr_t;
  using set_decorations = std::nullptr_t;
  using set_geometry = std::nullptr_t;
  using set_hints = std::nullptr_t;
  using set_opacity = std::nullptr_t;
  using set_override_redirect = std::nullptr_t;
  using set_parent = std::nullptr_t;
  using set_role = std::nullptr_t;
  using set_startup_id = std::nullptr_t;
  using set_strut_partial = std::nullptr_t;
  using set_title = std::nullptr_t;
  using set_window_type = std::nullptr_t;
} // namespace wlr_xwayland_surface
} // namespace wlr::events
