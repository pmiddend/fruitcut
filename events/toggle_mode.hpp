#ifndef FRUITCUT_EVENTS_TOGGLE_MODE_HPP_INCLUDED
#define FRUITCUT_EVENTS_TOGGLE_MODE_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace events
{
class toggle_mode
:
	public boost::statechart::event<toggle_mode>
{
public:
};
}
}

#endif
