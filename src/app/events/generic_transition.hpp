#ifndef FRUITCUT_APP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
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
}

#endif
