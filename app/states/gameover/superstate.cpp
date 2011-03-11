#include "superstate.hpp"
#include <sge/time/second_f.hpp>
#include <sge/time/second.hpp>

fruitcut::app::states::gameover::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	frame_timer_(
		sge::time::second(
			1)),
	gui_keyboard_(
		context<machine>().gui_syringe(),
		*context<machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<machine>().gui_syringe(),
		*context<machine>().systems().cursor_demuxer()),
	name_(
		"You shouldn't see this")
{
}

boost::statechart::result
fruitcut::app::states::gameover::superstate::react(
	events::render const &)
{
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::superstate::react(
	events::render_overlay const &)
{
	context<machine>().gui_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::superstate::react(
	events::tick const &)
{
	context<machine>().gui_system().update(
		sge::time::second_f(
			frame_timer_.reset()));
	return discard_event();
}

void
fruitcut::app::states::gameover::superstate::name(
	CEGUI::String const &_name)
{
	name_ = 
		_name;
}

CEGUI::String const &
fruitcut::app::states::gameover::superstate::name() const
{
	return name_;
}

fruitcut::app::states::gameover::superstate::~superstate()
{
}
