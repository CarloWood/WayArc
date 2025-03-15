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

