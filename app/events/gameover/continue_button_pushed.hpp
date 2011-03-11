#ifndef FRUITCUT_APP_EVENTS_GAMEOVER_CONTINUE_BUTTON_PUSHED_HPP_INCLUDED
#define FRUITCUT_APP_EVENTS_GAMEOVER_CONTINUE_BUTTON_PUSHED_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace fruitcut
{
namespace app
{
namespace events
{
namespace gameover
{
class continue_button_pushed
:
	public boost::statechart::event<continue_button_pushed>
{
};
}
}
}
}

#endif
