#ifndef FRUITCUT_APP_EVENTS_RENDER_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_RENDER_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
class render
:
	public boost::statechart::event<render>
{
};
}
}
}

#endif
