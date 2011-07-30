#include "superstate.hpp"
#include "../../depths/overlay.hpp"
#include <sge/systems/instance.hpp>

fruitcut::app::states::gameover::superstate::superstate(
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
	name_(
		"You shouldn't see this")
{
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
