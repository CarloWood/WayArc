#pragma once

#include "wl/Listener.h"
#include <memory>
#include <list>
#include "debug.h"
#ifdef CWDEBUG
#include "debug_ostream_operators.h"
#endif

namespace wlr {
class EventClient;

// Base class for event containers.
//
// This base class handles linking the wlroots events to the aicxx events API.
class EventsContainer
{
 private:
  std::list<std::unique_ptr<wl::ListenerBase>> listeners_;

 public:
  template<typename EVENT_TYPE>
  typename wl::Listener<EVENT_TYPE>& realize();

 protected:
  friend class EventClient;
  void destroy_listeners()
  {
    listeners_.clear();
  }
};

// Return a reference to the associated wl::Listener<EVENT_TYPE> of this EVENT_TYPE, creating it if it doesn't already exist.
template<typename EVENT_TYPE>
typename wl::Listener<EVENT_TYPE>& EventsContainer::realize()
{
  DoutEntering(dc::events, "EventsContainer<" << print_type<EVENT_TYPE> << ">::realize()");

  auto iter = std::find_if(listeners_.begin(), listeners_.end(),
      [](std::unique_ptr<wl::ListenerBase> const& elem){ return dynamic_cast<wl::Listener<EVENT_TYPE>*>(elem.get()) != nullptr; });
  if (iter == listeners_.end())
  {
    using event_info_type = wl::EventInfo<EVENT_TYPE::signal_enum>;
    using events_container_type = event_info_type::events_container_type;
    events_container_type* events_container = static_cast<events_container_type*>(this);
    wl_signal* signal_ptr = events_container->template get_signal_ptr<EVENT_TYPE::signal_enum>();
    iter = listeners_.emplace(listeners_.end(), std::make_unique<wl::Listener<EVENT_TYPE>>(signal_ptr));
  }
  return static_cast<wl::Listener<EVENT_TYPE>&>(**iter);
}

} // namespace wlr

// Internal macros used to declare signal-name structs that declare the corresponding wl::EventType.

#define __WAYARC_DECLARE_SIGNAL_HOOK(r, events_container_name, signal_name) \
__WAYARC_NL_MACRO__  struct signal_name { using event_type = wl::EventType<::events::events_container_name::signal_name>; };

#define __WAYARC_DECLARE_SIGNAL_HOOKS(events_container_name, signal_names_seq) \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_SIGNAL_HOOK, events_container_name, signal_names_seq)

// Internal macros used to declare elements of an enum class representing signals.

#define __WAYARC_DECLARE_SIGNAL_ENUM(r, events_container_name, signal_name) \
__WAYARC_NL_MACRO__  signal_name,

#define __WAYARC_DECLARE_SIGNAL_ENUMS(events_container_name, signal_names_seq) \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_SIGNAL_ENUM, events_container_name, signal_names_seq)

// Internal macros used to generate AI_CASE_RETURN statements to print those enum class elements.

#define __WAYARC_DECLARE_SIGNAL_ENUM_CASE_RETURN(r, events_container_name, signal_name) \
__WAYARC_NL_MACRO__     AI_CASE_RETURN(events_container_name::signal_name);

#define __WAYARC_DECLARE_SIGNAL_ENUM_CASE_RETURNS(events_container_name, signal_names_seq) \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_SIGNAL_ENUM_CASE_RETURN, events_container_name, signal_names_seq)

// Internal macros used to define specializations of the get_signal_ptr member function of event containers in namespace wlr, providing a pointer to the underlying wl_signal.

#define __WAYARC_DECLARE_GET_SIGNAL_PTR_SPECIALIZATION(r, events_container_tuple, signal_name_data_type) \
__WAYARC_NL_MACRO__  template<> \
__WAYARC_NL_MACRO__  wl_signal* BOOST_PP_TUPLE_ELEM(0, events_container_tuple)::get_signal_ptr<::events::BOOST_PP_TUPLE_ELEM(1, events_container_tuple)::BOOST_PP_TUPLE_ELEM(0, signal_name_data_type)>() const \
__WAYARC_NL_MACRO__  { \
__WAYARC_NL_MACRO__    ASSERT(ptr_); /* The wlr::BOOST_PP_TUPLE_ELEM(0, events_container_tuple) wasn't initialized. */ \
__WAYARC_NL_MACRO__    return &ptr_->events.BOOST_PP_TUPLE_ELEM(0, signal_name_data_type); \
__WAYARC_NL_MACRO__  }

#define __WAYARC_DECLARE_GET_SIGNAL_PTR_SPECIALIZATIONS(events_container, events_container_name, signal_name_data_type_seq) \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_GET_SIGNAL_PTR_SPECIALIZATION, (events_container, events_container_name), signal_name_data_type_seq)

// Internal macros used to define specializations of EventInfo, providing the underlying data types and enum value used by a given signal.

#define __WAYARC_DECLARE_EVENT_INFO_SPECIALIZATION(r, events_container_tuple, signal_name_data_type) \
__WAYARC_NL_MACRO__  template<> \
__WAYARC_NL_MACRO__  struct EventInfo<::events::BOOST_PP_TUPLE_ELEM(1, events_container_tuple)::BOOST_PP_TUPLE_ELEM(0, signal_name_data_type)> \
__WAYARC_NL_MACRO__  { \
__WAYARC_NL_MACRO__    using wlr_events_container_type = struct BOOST_PP_CAT(wlr_, BOOST_PP_TUPLE_ELEM(1, events_container_tuple)); \
__WAYARC_NL_MACRO__    static constexpr auto signal_enum = ::events::BOOST_PP_TUPLE_ELEM(1, events_container_tuple)::BOOST_PP_TUPLE_ELEM(0, signal_name_data_type); \
__WAYARC_NL_MACRO__    using event_data_type = BOOST_PP_TUPLE_ELEM(1, signal_name_data_type); \
__WAYARC_NL_MACRO__    using events_container_type = wlr::BOOST_PP_TUPLE_ELEM(0, events_container_tuple); \
__WAYARC_NL_MACRO__  };

#define __WAYARC_DECLARE_EVENT_INFO_SPECIALIZATIONS(events_container, events_container_name, signal_name_data_type_seq) \
  BOOST_PP_SEQ_FOR_EACH(__WAYARC_DECLARE_EVENT_INFO_SPECIALIZATION, (events_container, events_container_name), signal_name_data_type_seq)

#ifndef __WAYARC_NL_MACRO__
#define __WAYARC_NL_MACRO__
#endif
