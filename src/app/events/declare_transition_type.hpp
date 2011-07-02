#ifndef FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE_HPP_INCLUDED

#include "generic_transition.hpp"
#include <boost/statechart/custom_reaction.hpp>

#define FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(target_state)\
boost::statechart::custom_reaction\
<\
	fruitcut::app::events::generic_transition< target_state > \
>

#endif
