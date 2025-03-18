#pragma once

#include <cstddef>      // std::nullptr_t

// If the struct wlr_XYZ contains
//
//   struct wlr_XYZ {
//     struct {
//       wl_signal destroy;
//       wl_signal new_input;
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
