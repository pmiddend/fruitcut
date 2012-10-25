#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/states/menu/superstate.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::states::menu::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	escape_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				std::tr1::bind(
					&fruitapp::machine::quit,
					&context<fruitapp::machine>(),
					awl::main::exit_success()))))
{
	context<machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("menu")));
}

fruitapp::states::menu::superstate::~superstate()
{
}