#ifndef FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR_HPP_INCLUDED
#define FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR_HPP_INCLUDED

#include <fruitapp/events/generic_transition.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


#define FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(target_state)\
	std::bind(\
		&fruitapp::machine::post_event,\
		&context<fruitapp::machine>(),\
		fruitapp::events::generic_transition<fruitapp::states::target_state>())

#endif
