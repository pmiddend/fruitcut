#ifndef FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE_HPP_INCLUDED
#define FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE_HPP_INCLUDED

#include <fruitapp/events/generic_transition.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <fcppt/config/external_end.hpp>


#define FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(target_state)\
boost::statechart::custom_reaction\
<\
	fruitapp::events::generic_transition< target_state > \
>

#endif
