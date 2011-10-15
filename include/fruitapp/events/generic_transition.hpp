#ifndef FRUITAPP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED
#define FRUITAPP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitapp
{
namespace events
{
template<typename Target>
class generic_transition
:
	public boost::statechart::event<generic_transition<Target> >
{
};
}
}

#endif
