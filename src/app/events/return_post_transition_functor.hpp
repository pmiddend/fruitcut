#ifndef FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR_HPP_INCLUDED

#include "generic_transition.hpp"
#include <fcppt/tr1/functional.hpp>

#define FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(target_state)\
	std::tr1::bind(\
		&fruitcut::app::machine::post_event,\
		&context<fruitcut::app::machine>(),\
		fruitcut::app::events::generic_transition<fruitcut::app::states::target_state>())

#endif
