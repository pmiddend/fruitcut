#include "superstate.hpp"
#include "main.hpp"
#include "../../../fruitlib/audio/music_controller.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include <fcppt/text.hpp>
#include <sge/systems/instance.hpp>

fruitcut::app::states::menu::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	gui_node_(
		context<machine>().gui_system(),
		context<machine>().timer_callback()),
	gui_keyboard_(
		context<machine>().gui_syringe(),
		context<machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<machine>().gui_syringe(),
		context<machine>().systems().cursor_demuxer())
{
	context<machine>().overlay_node().insert_dont_care(
		gui_node_);

	context<machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("menu")));
}

fruitcut::app::states::menu::superstate::~superstate()
{
}
