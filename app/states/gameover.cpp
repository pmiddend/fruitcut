#include "gameover.hpp"
#include "../../media_path.hpp"
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>
#include <fcppt/text.hpp>

fruitcut::app::states::gameover::gameover(
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
	current_layout_(
		new sge::cegui::toolbox::scoped_layout(
			media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout"),
			context<machine>().systems().charconv_system()))
{
}

boost::statechart::result
fruitcut::app::states::gameover::react(
	events::render const &)
{
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::react(
	events::render_overlay const &)
{
	context<machine>().gui_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::react(
	events::tick const &)
{
	context<machine>().gui_system().update(
		sge::time::second_f(
			frame_timer_.reset()));
	return discard_event();
}

fruitcut::app::states::gameover::~gameover()
{
}
