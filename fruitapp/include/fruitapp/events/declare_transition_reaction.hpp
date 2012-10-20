#ifndef FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED
#define FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION_HPP_INCLUDED

#include <fruitapp/events/generic_transition.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/result.hpp>
#include <fcppt/config/external_end.hpp>


#define FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(target_state)\
boost::statechart::result \
react(\
	fruitapp::events::generic_transition< fruitapp::states::target_state > const &)

#endif
