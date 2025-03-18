#pragma once

#include "wl/Listener.h"
#include "wlr/events.h"
#include "utils/arrow_type.h"
#include <boost/preprocessor/seq/for_each.hpp>
#include <type_traits>

// Declare an event-server for the wlr event `signal_name` emitted by the struct `wlr_##events_container_name`.
//
// If the events container struct is for example `wlr_xyz` (the first argument of the macro is `xyz`)
// and the name of the signal is -say- `destroy`, then the name of the event-server will be `xyz_destroy_`.
//
// Example usage:
//
//   class Foo {
//     WA_DECLARE_EVENT_SERVER(seat, keyboard_grab_begin);        // Declares the event-server seat_keyboard_grab_begin_
//
#define WA_DECLARE_EVENT_SERVER(events_container_name, signal_name) \
  ::listener::wlr_##events_container_name::signal_name events_container_name##_##signal_name##_

// Declare an event-handle for the wlr event `signal_name` emitted by the struct `wlr_##events_container_name`.
//
// If the events container struct is for example `wlr_xyz` (the first argument of the macro is `xyz`)
// and the name of the signal is -say- `destroy`, then the name of the event-handle will be `xyz_destroy_handle_`.
//
// Example usage:
//
//   WA_DECLARE_EVENT_HANDLE(seat, keyboard_grab_begin);        // Declares the event-handle seat_keyboard_grab_begin_handle_;
//
// or
//
//   WA_DECLARE_EVENT_HANDLE(seat, keyboard_grab_begin) = seat_keyboard_grab_begin_.request(callback);
//
// but in that case you should probably just use auto, as the type is whatever `request` returns anyway.
// Aka,
//
//   auto seat_keyboard_grab_begin_handle_ = seat_keyboard_grab_begin_.request(callback);
//
#define WA_DECLARE_EVENT_HANDLE(events_container_name, signal_name) \
  ::listener::wlr_##events_container_name::signal_name::handle_t events_container_name##_##signal_name##_handle_

// Convenience macro to combine the above two macros into one.
#define WA_DECLARE_EVENT_SIMPLE(events_container_name, signal_name) \
  struct { \
    WA_DECLARE_EVENT_SERVER(events_container_name, signal_name); \
    WA_DECLARE_EVENT_HANDLE(events_container_name, signal_name); \
  }

// Convenience macro to combine WA_LINK_EVENT_SERVER and WA_REQUEST_EVENT_CALLBACK
// in case WA_DECLARE_EVENT_SIMPLE or WA_DECLARE_EVENT_SIMPLE_MEMBERS was used.
//
// If this macro used together with WA_DECLARE_EVENT_SIMPLE then also use
// WA_DECLARE_INIT_AND_REQUEST_CALLBACK below WA_DECLARE_EVENT_CALLBACK
// (this is not required when using WA_DECLARE_EVENT_SIMPLE_MEMBERS).
//
#define WA_LINK_EVENT_SIMPLE(events_container_name, signal_name, events_container, event_client) \
  __init_and_request_callback_##events_container_name##_##signal_name \
      (&events_container->events.signal_name, event_client, &std::remove_cvref_t<decltype(event_client)>::events_container_name##_##signal_name);

// Macro to declare an internal member function in case WA_LINK_EVENT_SIMPLE
// is used in combination with WA_DECLARE_EVENT_SIMPLE.
//
#define WA_DECLARE_INIT_AND_REQUEST_CALLBACK(events_container_name, signal_name) \
  template<typename __EventClient> \
  void __init_and_request_callback_##events_container_name##_##signal_name(wl_signal* signal, __EventClient& event_client, \
      void (__EventClient::*callback)(::listener::wlr_##events_container_name::signal_name::event_type_t const&)) { \
    events_container_name##_##signal_name##_.init(signal); \
    events_container_name##_##signal_name##_handle_ = events_container_name##_##signal_name##_.\
        request(event_client, callback); \
  }

// Declare a callback function suited for wlr event "(events_container_name, signal_name)".
//
// If the events container struct is for example `wlr_xyz` (the first argument of the macro is `xyz`)
// and the name of the signal is -say- `destroy`, then the signature of the callback function will
// be `void xyz_destroy(listener::wlr_xyz::destroy::event_type_t const& event_type);`.
//
// Example usage:
//
//   WA_DECLARE_EVENT_CALLBACK(seat, keyboard_grab_begin);
//
// If the callback is a free function, or an inline member function, the definition can
// immediately be appended. If it is a member function and the definition is elsewhere then
// use WA_DEFINE_EVENT_CALLBACK for the definition.
//
#define WA_DECLARE_EVENT_CALLBACK(events_container_name, signal_name) \
  void events_container_name##_##signal_name(::listener::wlr_##events_container_name::signal_name::event_type_t const& event_type)

// Convenience macro to combine WA_DECLARE_EVENT_SERVER and WA_DECLARE_EVENT_CALLBACK into one.
//
// It is recommended to use this in a private: code block and then
// call WA_LINK_EVENT_SERVER from a member function.
//
#define WA_DECLARE_EVENT_MEMBERS(events_container_name, signal_name) \
  WA_DECLARE_EVENT_SERVER(events_container_name, signal_name); \
  WA_DECLARE_EVENT_CALLBACK(events_container_name, signal_name)

// Convenience macro to combine WA_DECLARE_EVENT_SERVER, WA_DECLARE_EVENT_HANDLE and WA_DECLARE_EVENT_CALLBACK into one.
#define WA_DECLARE_EVENT_SIMPLE_MEMBERS(events_container_name, signal_name) \
  WA_DECLARE_EVENT_SIMPLE(events_container_name, signal_name); \
  WA_DECLARE_INIT_AND_REQUEST_CALLBACK(events_container_name, signal_name) \
  WA_DECLARE_EVENT_CALLBACK(events_container_name, signal_name)

// Definition header of a callback member function that was declared with WA_DECLARE_EVENT_CALLBACK.
//
// Example usage:
//
//   WA_DEFINE_EVENT_CALLBACK(MyClass, events_container_name, signal_name)
//   {
//     ...
//   }
//
// The function does not return anything.
#define WA_DEFINE_EVENT_CALLBACK(class, events_container_name, signal_name, event_type) \
  void class::events_container_name##_##signal_name(::listener::wlr_##events_container_name::signal_name::event_type_t const& event_type)

// Links an event-server, as declared with WA_DECLARE_EVENT_SERVER, with its underlying wlroots signal,
// if the event-server was default constructed (as opposed to immediately passing the wl_signal object).
//
// If the events container struct is for example `wlr_xyz` (the first argument of the macro is `xyz`)
// and the name of the signal is -say- `destroy`, then this macro evaluates to
//
//   xyz_destroy_.init(&events_container->events.destroy)
//
// where events_container must have type wlr_xyz* and is passed as the third argument of the macro.
//
// Example usage:
//
//   struct MyObject {
//     struct wlr_seat* wlr_seat_;
//     // Default construct the event-server `listener::wlr_seat::request_set_cursor wlr_seat_request_set_cursor_`.
//     WA_DECLARE_EVENT_SERVER(seat, request_set_cursor);
//     ...
//   };
//
//   // Initialize wlr_seat_request_set_cursor_ by linking it to the event `request_set_cursor` of what `wlr_seat_` points to.
//   my_object->WA_LINK_EVENT_SERVER(seat, request_set_cursor, my_object->wlr_seat_);
//
// Note: doing this inside a member function of my_object is recommended
//       as that does simplify this initialization to:
//
//   WA_LINK_EVENT_SERVER(seat, request_set_cursor, wlr_seat_);
//
#define WA_LINK_EVENT_SERVER(events_container_name, signal_name, events_container) \
  events_container_name##_##signal_name##_.init(&events_container->events.signal_name); \
  static_assert(std::disjunction<std::is_same<decltype(events_container), struct wlr_##events_container_name*>, \
      std::is_same<utils::arrow_type<decltype(events_container)>, struct wlr_##events_container_name*>>::value, \
      "The third parameter of WA_LINK_EVENT_SERVER must have type `wlr_" #events_container_name "*`.");

// Request a callback for the wlr event "(events_container_name, signal_name)".
//
// Example usage:
//
//   struct MyObject {
//     struct wlr_seat* wlr_seat_;
//     // Default construct the event-server `listener::wlr_seat::request_set_cursor wlr_seat_request_set_cursor_`.
//     WA_DECLARE_EVENT_SERVER(seat, request_set_cursor);
//     ...
//   };
//
//   // Request a callback from the event-server in my_object declared with WA_DECLARE_EVENT_SERVER(seat, request_set_cursor).
//   auto handle = my_object.WA_REQUEST_EVENT_CALLBACK(seat, request_set_cursor, my_object);
//
// This macro takes two or three arguments. If three arguments are provided then the last parameter must
// be the object containing the callback function as member function. Two arguments are sufficient if
// the callback was declared as a free function.
//
#define WA_REQUEST_EVENT_CALLBACK(...) \
    WA_REQUEST_EVENT_CALLBACK_SELECT(__VA_ARGS__, WA_REQUEST_EVENT_CALLBACK3, WA_REQUEST_EVENT_CALLBACK2)(__VA_ARGS__)

#define WA_REQUEST_EVENT_CALLBACK_SELECT(_1, _2, _3, NAME, ...) NAME

// Two arguments: callback is a free function.
#define WA_REQUEST_EVENT_CALLBACK2(events_container_name, signal_name) \
  events_container_name##_##signal_name##_.request(events_container_name##_##signal_name)

// Three arguments: callback is a member function of the third argument.
#define WA_REQUEST_EVENT_CALLBACK3(events_container_name, signal_name, event_client) \
  events_container_name##_##signal_name##_.request(event_client, &std::remove_cvref_t<decltype(event_client)>::events_container_name##_##signal_name)

// Conveniece macro to register event call back when using WA_DECLARE_EVENT_SIMPLE.
//
#define WA_REGISTER_EVENT_CALLBACK(events_container_name, signal_name) \
  events_container_name##_##signal_name##_handle_ = WA_REQUEST_EVENT_CALLBACK(events_container_name, signal_name, *this)

// Internal macros used below.

#define __WAYARC_DECLARE_LISTENER(r, event_class, signal_name) \
  using signal_name = wl::Listener<wlr::events::event_class::signal_name>;

#define __WAYARC_DECLARE_LISTENERS(event_class, events_seq) \
namespace event_class { \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_LISTENER, event_class, events_seq) \
}

// Declare all wl::Listener<> types in namespace listener.
//
// This file was generated. It contains a __WAYARC_DECLARE_LISTENERS(wlr_XYZ, (signal1)(signal2)...)
// for each struct wlr_XYZ that contains an `events` struct with wl_signal members signal1, signal2 etc.
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

// These __WAYARC_DECLARE_LISTENERS lines are generated with: events_generator/convert --listener analyzer.out :

