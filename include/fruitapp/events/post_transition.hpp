#ifndef FRUITAPP_EVENTS_POST_TRANSITION_HPP_INCLUDED
#define FRUITAPP_EVENTS_POST_TRANSITION_HPP_INCLUDED

#include <fruitapp/events/generic_transition.hpp>


#define FRUITAPP_EVENTS_POST_TRANSITION(target_state)\
	context<machine>().post_event(\
		fruitapp::events::generic_transition<fruitapp::states::target_state>());

#endif
