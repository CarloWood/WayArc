#pragma once

#include "events/Events.h"
#include "wl/EventType.h"
#include <vector>
#include <functional>
#include <utility>
#include "debug.h"
#ifdef CWDEBUG
// Forward declare events::cursor.
namespace events { enum class cursor; }
// Unfortunately we can't forward declare events::output::frame.
#include "wlr/Output.h"
#include <type_traits>
#endif

namespace wlr {

class EventClient
{
 private:
  std::vector<std::move_only_function<void()>> events_handle_cancels_;          // Stores one lambda per event handle that will cancel the event callback if invoked.
  std::vector<EventsContainer*> registered_event_containers_;                   // A list of EventsContainer objects for which register_event(events_container, ...) was called.

 public:
  // Non-const client.
  template<typename EVENTS_CONTAINER, typename EVENT_TYPE, class CLIENT, typename... Args>
  void register_event(EVENTS_CONTAINER& events_container, void (CLIENT::*cb)(EVENT_TYPE const&, Args...), Args... args)
  {
    static_assert(std::is_same_v<typename EVENTS_CONTAINER::wlr_events_container_type, typename wl::EventInfo<EVENT_TYPE::signal_enum>::wlr_events_container_type>,
        "Mismatch between wlr_events_container_type of passed EVENTS_CONTAINER and the EVENT_TYPE of the callback function!");
    DoutEntering(dc::events,
        "EventClient::register_event(" <<
            libcwd::type_info_of<EVENTS_CONTAINER&>().demangled_name() << " [@" << (void*)&events_container << "], " <<
            "void (" << libcwd::type_info_of<CLIENT>().demangled_name() << "::*)(" << wl::print_type<EVENT_TYPE>() << " (with EventData " <<
              libcwd::type_info_of<typename wl::EventInfo<EVENT_TYPE::signal_enum>::event_data_type>().demangled_name() << ") const&" <<
              ("" << ... << (", " << libcwd::type_info_of<Args>().demangled_name())) << ")" <<
            ("" << ... << (", ", args)) <<
        ")");

    // Remember all events_container's that we were being called with.
    auto iter = std::find(registered_event_containers_.begin(), registered_event_containers_.end(), static_cast<EventsContainer*>(&events_container));
    if (iter == registered_event_containers_.end())
      registered_event_containers_.push_back(&events_container);

    // Find or create the wl::Listener object associated with this event type.
    auto& listener = events_container.template realize<EVENT_TYPE>();
    // The client is derived from this object.
    CLIENT& client = static_cast<CLIENT&>(*this);
    // Request to be notified by a call to client.cb() when the event happens.
    events::RequestHandle<EVENT_TYPE> handle = listener.request(client, cb);
#ifdef CWDEBUG
    // Turn off debug output for callback that are called very frequently.
    if constexpr (std::is_same_v<std::remove_cvref_t<decltype(EVENT_TYPE::signal_enum)>, events::cursor>)
      handle.debug_set_silent();
    else if constexpr (std::is_same_v<std::remove_cvref_t<decltype(EVENT_TYPE::signal_enum)>, events::output>)
    {
      if (EVENT_TYPE::signal_enum == events::output::frame)
        handle.debug_set_silent();
    }
#endif
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
    for (EventsContainer* event_container : registered_event_containers_)
      event_container->destroy_listeners();
  }

  void add_event_handle_cancel(std::move_only_function<void()>&& event_handle_cancel)
  {
    events_handle_cancels_.push_back(std::move(event_handle_cancel));
  }
};

} // namespace wlr
