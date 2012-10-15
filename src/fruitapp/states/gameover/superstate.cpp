#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/states/gameover/superstate.hpp>
#include <sge/systems/instance.hpp>


fruitapp::states::gameover::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	gui_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		context<fruitapp::machine>().gui_system(),
		context<fruitapp::machine>().standard_clock_callback()),
	gui_keyboard_(
		context<fruitapp::machine>().gui_syringe(),
		context<fruitapp::machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<fruitapp::machine>().gui_syringe(),
		context<fruitapp::machine>().systems().cursor_demuxer()),
	name_(
		"You shouldn't see this")
{
}

void
fruitapp::states::gameover::superstate::name(
	CEGUI::String const &_name)
{
	name_ =
		_name;
}

CEGUI::String const &
fruitapp::states::gameover::superstate::name() const
{
	return name_;
}

fruitapp::states::gameover::superstate::~superstate()
{
}
