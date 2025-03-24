#pragma once

#include "events/Events.h"
#include "wl/EventType.h"
#include <vector>
#include <functional>
#include <utility>
#include "debug.h"

namespace wlr {

class EventClient
{
 private:
  std::vector<std::move_only_function<void()>> events_handle_cancels_;          // Stores one lambda per event handle that will cancel the event callback if invoked.

 public:
  // Non-const client.
  template<typename EVENTS_CONTAINER, typename EVENT_TYPE, class CLIENT, typename... Args>
    requires requires { std::is_same_v<typename EVENTS_CONTAINER::wlr_events_container_type, typename wl::EventInfo<EVENT_TYPE::signal_enum>::wlr_events_container_type>; }
  void register_event(EVENTS_CONTAINER& events_container, void (CLIENT::*cb)(EVENT_TYPE const&, Args...), Args... args)
  {
    DoutEntering(dc::events,
        "EventClient::register_event(" <<
            libcwd::type_info_of<EVENTS_CONTAINER&>().demangled_name() << " [@" << (void*)&events_container << "], " <<
            "void (" << libcwd::type_info_of<CLIENT>().demangled_name() << "::*)(" << wl::print_type<EVENT_TYPE>() << " (with EventData " <<
              libcwd::type_info_of<typename wl::EventInfo<EVENT_TYPE::signal_enum>::event_data_type>().demangled_name() << ") const&" <<
              ("" << ... << (", " << libcwd::type_info_of<Args>().demangled_name())) << ")" <<
            ("" << ... << (", ", args)) <<
        ")");

    // Find or create the wl::Listener object associated with this event type.
    auto& listener = events_container.template realize<EVENT_TYPE>();
    // The client is derived from this object.
    CLIENT& client = static_cast<CLIENT&>(*this);
    // Request to be notified by a call to client.cb() when the event happens.
    events::RequestHandle<EVENT_TYPE> handle = listener.request(client, cb);
    // The handle returned by `request` is added to the client; the destructor of
    // the client should call `cancel_events()`, which calls `cancel()` on all added handles.
    add_event_handle_cancel([handle = std::move(handle)]() mutable { handle.cancel(); });
  }

 protected:
  // Call this from the destructor of the most-derived class.
  void cancel_events()
  {
    for (auto& event_handle_cancel : events_handle_cancels_)
      event_handle_cancel();
  }

  void add_event_handle_cancel(std::move_only_function<void()>&& event_handle_cancel)
  {
    events_handle_cancels_.push_back(std::move(event_handle_cancel));
  }
};

} // namespace wlr
