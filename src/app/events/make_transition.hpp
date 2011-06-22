#ifndef FRUITCUT_APP_EVENTS_MAKE_TRANSITION_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_MAKE_TRANSITION_HPP_INCLUDED

#include "generic_transition.hpp"
#include <boost/statechart/transition.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
template<typename T>
struct make_transition
{
	typedef	
	boost::statechart::transition
	<
		events::generic_transition<T>,
		T
	>
	type;
};
}
}
}

#endif
