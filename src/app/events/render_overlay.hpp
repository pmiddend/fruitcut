#ifndef FRUITCUT_APP_EVENTS_RENDER_OVERLAY_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_RENDER_OVERLAY_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
class render_overlay
:
	public boost::statechart::event<render_overlay>
{
};
}
}
}

#endif
