#pragma once

#include "utils/has_print_on.h"
#include "utils/print_pointer.h"
#include "events/Events.h"
#include <wayland-server-core.h>
#include <utility>

namespace wl {
// This class defines a print_on method.
using utils::has_print_on::operator<<;

template<typename Data>
class EventType
{
 public:
  static constexpr bool one_shot = false;

 private:
  Data* data_;

 public:
  EventType(Data* data) : data_(data) { }

  Data* operator->() const { return data_; }
  operator Data*() const { return data_; }

#ifdef CWDEBUG
  void print_on(std::ostream& os) const;
#endif
};

#ifdef CWDEBUG
template<typename Data>
void EventType<Data>::print_on(std::ostream& os) const
{
  os << '{';
  os << "data_:" << utils::print_pointer(data_);
  os << '}';
}
#endif

struct SignalServerListener
{
  wl_listener listener_{};
};

template<typename Data>
class Listener : protected SignalServerListener, public events::Server<EventType<Data>>
{
 public:
  using event_type_t = EventType<Data>;
  using handle_t = events::RequestHandle<event_type_t>;

 private:
  wl_signal* signal_ptr_{};

 public:
  Listener() = default;
  Listener(wl_signal* signal_ptr) { init(); }
  ~Listener();

  void init(wl_signal* signal_ptr);

 private:
  static void callback_(wl_listener* listener, void* data);
};

template<typename Data>
void Listener<Data>::init(wl_signal* signal_ptr)
{
  // Only call init once.
  // Did you do a WA_LINK_EVENT_SERVER twice for the same event-server?
  ASSERT(signal_ptr_);
  signal_ptr_ = signal_ptr;

  listener_.notify = &Listener<Data>::callback_;
  wl_signal_add(signal_ptr_, &listener_);
}

template<typename Data>
Listener<Data>::~Listener()
{
  if (listener_.link.prev != nullptr)   // Was init() called?
    wl_list_remove(&listener_.link);
}

//static
template<typename Data>
void Listener<Data>::callback_(wl_listener* listener, void* data_ptr)
{
  SignalServerListener* base = wl_container_of(listener, std::declval<SignalServerListener*>(), listener_);
  Listener<Data>* self = static_cast<Listener<Data>*>(base);

  Data* data = reinterpret_cast<Data*>(data_ptr);
  EventType<Data> event_type(data);
  self->trigger(event_type);
}

} // namespace wl
