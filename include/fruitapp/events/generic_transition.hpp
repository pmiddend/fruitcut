#ifndef FRUITAPP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED
#define FRUITAPP_EVENTS_GENERIC_TRANSITION_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/event.hpp>
#include <fcppt/config/external_end.hpp>


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
