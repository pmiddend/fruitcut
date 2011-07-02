#ifndef FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED

#include "generic_transition.hpp"
#include <boost/statechart/result.hpp>

#define FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(target_state)\
boost::statechart::result \
react(\
	fruitcut::app::events::generic_transition< fruitcut::app::states::target_state > const &)

#endif
