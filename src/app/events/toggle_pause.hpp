#ifndef FRUITCUT_APP_EVENTS_TOGGLE_PAUSE_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_TOGGLE_PAUSE_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
class toggle_pause
:
	public boost::statechart::event<toggle_pause>
{
};
}
}
}

#endif
