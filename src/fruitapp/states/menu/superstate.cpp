#include <fruitapp/states/menu/superstate.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/device.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>

fruitapp::states::menu::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	gui_node_(
		fruitlib::scenic::parent(
			context<machine>().overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		context<machine>().gui_system()),
	gui_keyboard_(
		context<machine>().gui_syringe(),
		context<machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<machine>().gui_syringe(),
		context<machine>().systems().cursor_demuxer()),
	escape_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				std::tr1::bind(
					&fruitapp::machine::quit,
					&context<fruitapp::machine>()))))
{
	context<machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("menu")));
}

fruitapp::states::menu::superstate::~superstate()
{
}
