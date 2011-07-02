#ifndef FRUITCUT_APP_EVENTS_POST_TRANSITION_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_POST_TRANSITION_HPP_INCLUDED

#include "generic_transition.hpp"

#define FRUITCUT_APP_EVENTS_POST_TRANSITION(target_state)\
	context<machine>().post_event(\
		fruitcut::app::events::generic_transition<fruitcut::app::states::target_state>());

#endif
