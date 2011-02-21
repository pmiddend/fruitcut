#ifndef FRUITCUT_APP_EVENTS_VIEWPORT_CHANGE_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_VIEWPORT_CHANGE_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
class viewport_change
:
	public boost::statechart::event<viewport_change>
{
};
}
}
}

#endif
