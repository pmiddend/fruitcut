#include "ingame.hpp"
#include "running.hpp"
#include "../../input/state.hpp"
#include "../events/toggle_pause.hpp"
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <boost/bind.hpp>
#include <boost/statechart/event_base.hpp>

fruitcut::app::states::ingame::ingame(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				boost::bind(
					// Fucking missing overload resolution
					static_cast<void (ingame::*)(boost::statechart::event_base const &)>(
						&ingame::post_event),
					this,
					events::toggle_pause()))))
{
}

fruitcut::app::states::ingame::~ingame()
{
}
