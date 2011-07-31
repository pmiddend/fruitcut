#ifndef FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED
#define FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED

#include "generic_transition.hpp"
#include <boost/statechart/result.hpp>

#define FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(target_state)\
boost::statechart::result \
react(\
	fruitapp::events::generic_transition< fruitapp::states::target_state > const &)

#endif
