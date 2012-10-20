#ifndef FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION_HPP_INCLUDED
#define FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION_HPP_INCLUDED

#define FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(target_state,source_state)\
boost::statechart::result \
fruitapp::states::source_state::react(\
	events::generic_transition<target_state> const &)\
{\
	return transit<target_state>();\
}

#endif
