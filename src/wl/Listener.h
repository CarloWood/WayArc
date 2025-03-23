#pragma once

#include "wl/EventType.h"
#include "utils/has_print_on.h"
#include "utils/print_pointer.h"
#include "events/Events.h"
#include <wayland-server-core.h>
#include <utility>

namespace wl {

struct SignalServerListener
{
  wl_listener listener_{};
};

struct ListenerBase
{
  // Allow using std::unique_ptr<ListenerBase> to point to the Listener objects.
  virtual ~ListenerBase() = default;
};

template<typename EVENT_TYPE>
class Listener : protected SignalServerListener, public events::Server<EVENT_TYPE>, public ListenerBase
{
 public:
  using event_type_t = EVENT_TYPE;
  using handle_t = events::RequestHandle<event_type_t>;

 private:
  wl_signal* signal_ptr_{};

 public:
  Listener() = default;
  Listener(wl_signal* signal_ptr) { init(signal_ptr); }
  ~Listener();

  void init(wl_signal* signal_ptr);

 private:
  static void callback_(wl_listener* listener, void* data);
};

template<typename EVENT_TYPE>
void Listener<EVENT_TYPE>::init(wl_signal* signal_ptr)
{
  // Only call init once.
  // Did you do a WA_LINK_EVENT_SERVER twice for the same event-server?
  ASSERT(signal_ptr_ == nullptr);
  signal_ptr_ = signal_ptr;

  listener_.notify = &Listener<EVENT_TYPE>::callback_;
  wl_signal_add(signal_ptr_, &listener_);
}

template<typename EVENT_TYPE>
Listener<EVENT_TYPE>::~Listener()
{
  if (listener_.link.prev != nullptr)   // Was init() called?
    wl_list_remove(&listener_.link);
}

//static
template<typename EVENT_TYPE>
void Listener<EVENT_TYPE>::callback_(wl_listener* listener, void* data_ptr)
{
  SignalServerListener* base = wl_container_of(listener, std::declval<SignalServerListener*>(), listener_);
  Listener<EVENT_TYPE>* self = static_cast<Listener<EVENT_TYPE>*>(base);

  typename EVENT_TYPE::data_type* data = reinterpret_cast<typename EVENT_TYPE::data_type*>(data_ptr);
  EVENT_TYPE event_type(data);
  self->trigger(event_type);
}

} // namespace wl
